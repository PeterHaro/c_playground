#include <stdlib.h>
#include <errno.h>

#include "list.h"

/*
 * List implemented as a doubly linked list
 */
#pragma region list_structure
struct listnode;
typedef struct listnode listnode_t;

struct listnode {
    listnode_t *next, *previous;
    void *element;
};

struct list {
    listnode_t *head, *tail;
    int numberOfElements;
    compare_function_t compare_function;
};
#pragma endrgion list_structure

static listnode_t *listnode_create(void *element) {
    listnode_t *node = (listnode_t *) malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->next = node->previous = NULL;
    node->element = element;
    return node;
}

static void list_deletenode(list_t *list, listnode_t *node) {
    free(node);
    list->numberOfElements--;
}

list_t *list_create(compare_function_t compare_function) {
    list_t *list = (list_t *) malloc(sizeof(*list));
    if (list == NULL) {
        return NULL;
    }
    list->head = list->tail = NULL;
    list->numberOfElements = 0;
    return list;
}

void list_destroy(list_t *list) {
    while (list_pop(list) != NULL) { ;;
    }
    free(list);
}

int list_append(list_t *list, void *element) {
    if (list == NULL || element == NULL) {
        return EINVAL;
    }

    listnode_t *new_node = listnode_create(element);
    if (new_node == NULL) {
        return errno;
    }

    if (list->head == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->previous = list->tail;
        list->tail = new_node;
    }
    list->numberOfElements++;
    return 0;
}

int list_prepend(list_t *list, void *element) {
    if (list == NULL || element == NULL) {
        return EINVAL;
    }

    listnode_t *new_node = listnode_create(element);
    if (new_node == NULL) {
        return errno;
    }

    if (list->head == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->head->previous = new_node;
        new_node->next = list->head;
        list->head = new_node;
    }
    list->numberOfElements++;
    return 0;
}

int list_remove(list_t *list, void *element) {
    if (list == NULL || element == NULL) {
        return EINVAL;
    }

    listnode_t *node = NULL;
    if (list->head == NULL) {
        return ENOENT; //EMPTY list
    }

    node = list->head;
    if (list->head->element == element) {
        //Well ok, that was easy
        list->head = node->next;
        if (list->head == NULL) {
            list->tail = NULL;
        } else {
            list->head->previous;
        }
    } else {
        while (node != NULL && node->element != element) {
            node = node->next;
        }
        if (node == NULL) {
            return ENOENT;
        }

        node->previous->next = node->next;
        if (node->next == NULL) {
            list->tail = node->previous;
        } else {
            node->next->previous = node->previous;
        }
    }

    list_deletenode(list, node);
    return 0;
}

void *list_shift(list_t *list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }

    listnode_t *headnode = list->head;
    if (headnode->next == NULL) {
        list->tail = NULL;
    } else {
        headnode->next->previous = NULL;
    }
    list->head = headnode->next;

    void *retval = headnode->element;
    list_deletenode(list, headnode);
    return retval;
}

void *list_pop(list_t *list) {
    if (list == NULL || list->tail == NULL) {
        return NULL;
    }

    listnode_t *tailnode = list->tail;
    if (tailnode->previous == NULL) {
        list->head = NULL;
    } else {
        tailnode->previous->next = NULL;
    }

    list->tail = tailnode->previous;
    void *retval = tailnode->element;
    list_deletenode(list, tailnode);
    return retval;
}

int list_size(list_t *list) {
    return list == NULL ? -EINVAL : list->numberOfElements;
}

int list_contains(list_t *list, void *element) {
    listnode_t *node = list->head;
    while (node != NULL) {
        if (list->compare_function(element, node->element) == 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

#pragma region iterator

struct list_iterator {
    listnode_t *previous, *next;
    list_t *list;
};

list_iterator_t *list_iterator_create(list_t *list) {
    if (list == NULL) {
        return NULL;
    }

    list_iterator_t *list_iterator = (list_iterator_t *) malloc(sizeof(sizeof(*list_iterator)));
    if (list_iterator == NULL) {
        return NULL;
    }

    list_iterator->list = list;
    list_iterator_reset(list_iterator);
    return list_iterator;
}

void list_iterator_destroy(list_iterator_t *iter) {
    free(iter);
}

int list_iterator_hasnext(list_iterator_t *iter) {
    return iter->next == NULL ? 0 : 1;
}

int list_iterator_hasprevious(list_iterator_t *iter) {
    return iter->previous == NULL ? 0 : 1;
}

void *list_iterator_next(list_iterator_t *iter) {
    if (iter == NULL || iter->next == NULL) {
        return NULL;
    }

    void *retval = iter->next->element;
    iter->previous = iter->next;
    iter->next = iter->next->next;

    return retval;
}

void *list_iterator_previous(list_iterator_t *iter) {
    if (iter == NULL || iter->previous == NULL) {
        return NULL;
    }

    void *retval = iter->previous->element;
    iter->next = iter->previous;
    iter->previous = iter->previous->previous;

    return retval;
}


int list_iterator_reset(list_iterator_t *iter) {
    if (iter == NULL) {
        return EINVAL;
    }
    iter->next = iter->list->head;
    iter->previous = iter->list->tail;
    return 0;
}
#pragma endregion iterator