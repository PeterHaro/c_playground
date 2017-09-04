#ifndef C_PLAYGROUND_UTILITY_H
#define C_PLAYGROUND_UTILITY_H

/*
 * The type of comparison functions.
 *  * returns:
 *   <0 <-- a < b
 *   0 <-- a == b
 *   >0 <-- a > b
 */
typedef int (*compare_function_t)(void const *, void const *);

#endif //C_PLAYGROUND_UTILITY_H
