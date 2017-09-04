#ifndef C_PLAYGROUND_LIST_H
#define C_PLAYGROUND_LIST_H
//Blast to the past. Creating a list interface

#include "utility.h"

/*
 * The (generic) list data structure
 */
struct list;
typedef struct list list_t;

/*
 * Creates a new, empty list that uses the given comparison function
 * to compare elements.
 *
 * Returns:
 *  a new empty list *list_t
 *  or NULL on error
 *
 * Returns the new list.
 */
extern struct list *list_create(compare_function_t compare_function);

/*
 * Destroys the list unless the list is NULL. Then ignore.
 */
extern void list_destroy(list_t *list);

/*
 * Append an element to the list, we avoid deep-copying, so duplicates are allowed.
 *
 * Returns:
 *  0 on success
 *  If the list or element is invalid (NULL), EINVAL is returned
 *  Otherwise errno
 */
extern int list_append(list_t *list, void *element);

/*
 * Prepend an element to the list, we avoid deep-copying, so duplicates are allowed.
 *
 * Returns:
 *  0 on success
 *  If the list or element is invalid (NULL), EINVAL is returned
 *  Otherwise errno
 */
extern int list_prepend(list_t *list, void *element);

/*
 * Removes a given element from the list
 *
 * Returns:
 *   0 on success
 *   If the list or element is invalid (NULL), EINVAL is returned
 *   Otherwise ENOENT for elements not in thee list
 */
extern int list_remove(list_t *list, void *element);

/*
 * Removes and returns the first element of the given list.
 *
 * Returns:
 *  NULL on NULL or empty list
 *  Otherwise returns the elemeent
 */
extern void *list_shift(list_t *list);

/*
 * Removes and returns the last element of the given list.
 * Returns:
 *  NULL on NULL or empty list
 *  Otherwise returns the elemeent
 */
extern void *list_pop(list_t *list);

//========================================================================
//                          UTILITY
//========================================================================


/*
 * Gets the number of elements in the list
 *
 * returns:
 *   Returns the current size of the given list.
 *   If the list is NULL returns -EINVAL
 */
extern int list_size(list_t *list);

/*
 * Determines whether the list contains a given element
 *
 * Returns true if found, otherwise false. Although the comparator function is used to check for equality we do not differentiate between true and false equality based on equivalence.
 */
extern int list_contains(list_t *list, void *element);

/*
 *  TODO: Figure out whether sort should stay in the list API, or if I can do something alla other languages with a utility function I.E. Colllections.sort(list_t *list);
 * Sorts the elements of the given list, using the comparison function
 * of the list to determine the ordering of the elements.
 */
extern void list_sort(list_t *list);

/*
 * The type of list iterators.
 */
struct list_iterator;
typedef struct list_iterator list_iterator_t;

/*
 * Creates a new list iterator for iterating over the given list.
 */
extern list_iterator_t *list_iterator_create(list_t *list);

/*
 * Destroys the given list iterator. If NULL it does nothing
 */
extern void list_iterator_destroy(list_iterator_t *iter);

/*
 * Returns 0 if the given list iterator has reached the end of the
 * list, or 1 otherwise.
 */
extern int list_iterator_hasnext(list_iterator_t *iter);

/*
 * Returns 0 if the given list iterator has reached the end of the
 * list, or 1 otherwise.
 */
extern int list_iterator_hasprevious(list_iterator_t *iter);

/*
 * Progress the list iterator and return the next element in the sequence represented by the given
 * list iterator.
 *
 * Returns:
 * The next element or
 *  Null if the list iterator is NULL or EndOfList
 */
extern void *list_iterator_next(list_iterator_t *iter);

/*
 * Regress the list iterator and return the next element in the sequence represented by the given
 * list iterator.
 *
 * Returns:
 * The previous element or
 *  Null if the list iterator is NULL or EndOfList
 */
extern void *list_iterator_previous(list_iterator_t *iter);

/*
 * Resets the given list iterator
 *
 * returns:
 *   Returns 0 on success
 *   Otherwise return EINVAL
 */
extern int list_iterator_reset(list_iterator_t *iter);

#endif //C_PLAYGROUND_LIST_H
