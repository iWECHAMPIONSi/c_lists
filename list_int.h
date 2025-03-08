#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <float.h>

#define DOUBLE_MIN DBL_MIN
#define DOUBLE_MAX DBL_MAX

typedef struct list_int {
 bool frozen; /**< if the capacity of the array is allowed to change */
	bool readonly; /**< if the data of the array is allowed to change */
	bool static_; /**< similar to readonly, although also does not allow for destruction*/
	unsigned int size; /**< Current number of elements */
	unsigned int capacity; /**< Maximum number of elements before resizing */
	int *data; /**< Pointer to the array of elements */
} list_int;

/**
 * A mutable list containing int
 *
 * Example:
 * @code
 * list_int *my_list = init_list_int(5);
 * @endcode
 *
 * @param init_size the initial size of the list, must be greater than 0
 *
 * @return success state:
 * - pointer to list_int: successfully created a new list_int
 * - nullptr: Memory error
 *  - Allocation for list failed
 *  - Allocation for list->data failed
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
list_int *init_list_int(unsigned int init_size);


/**
 * initializes a list_int with the values from an arr
 *
 * Example:
 * @code
 * int my_arr[3];
 * for (int i = 0; i < 3; i++)
 * {
 *     my_arr[i] = i;
 * }
 * list_int *my_list = init_list_arr_int(my_arr, 3);
 * @endcode
 *
 * @param arr pointer to arr of int, cannot be nullptr
 * @param size size of arr, cannot be 0
 *
* @return success state:
 * - pointer to list_int: successfully created a new list_int
 * - nullptr: Parameter error
 *  - arr was nullptr
 * - nullptr: Memory error
 *  - Allocation in function init_list_int failed
 *
 * @note complexity:
 * - best case: O(m)
 * - worst case: O(m)
 */
list_int *init_list_arr_int(int *arr, unsigned int size);

/**
 * splits a list at specified point
 * [index, end_list]
 *
 * Example:
 * @code
 * // my_list = {0, 1, 2, 3, 4, 5, 6}
 * list_int *split = list_split_int(my_list, -1);
 * // my_list = {0, 1, 2, 3}
 * // split = {4, 5, 6}
 * @endcode
 *
 * @param list a pointer to a list_int
 * @param index the inclusive index to split at, or -1 to get the center
 *  - if list->size % 2 = 1 then the split point is list->size / 2 + 1
 *
 * @return success state
 * - *list_int: Function executed successfully
 * - nullptr: Parameter error
 *  - list->size = 0 (empty list)
 *  - list->size = 1 (cannot split without having an empty list)
 *  - index = 0 (cannot split without having an empty list)
 *  - Index out of bounds
 * - nullptr: Memory error (no values changed)
 *  - Memory allocation failed
 * - nullptr: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n - m) amortized
 * - worst case: O(n - m) amortized
 */
list_int *list_split_int(list_int *list, int index);

/**
 * splits a list at specified point
 * [index, end_list]
 *
 * Example:
 * @code
 * // my_list = {0, 1, 2, 3, 4, 5, 6}
 * unsigned int size;
 * int *arr;
 * list_split_int(my_list, -1, &arr, &size);
 * // my_list = {0, 1, 2, 3}
 * // arr = {4, 5, 6}
 * // size = 3
 * @endcode
 *
 * @param list a pointer to a list_int
 * @param index the inclusive index to split at, or -1 to get the center
 *  - if list->size % 2 = 1 then the center point is list->size / 2 + 1
 * @param arr address to a NULL int arr
 * @param size address to return the size
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - list->size = 0 (empty list)
 *  - list->size = 1 (cannot split without having an empty list)
 *  - index = 0 (cannot split without having an empty list)
 *  - Index out of bounds
 *  - arr is nullptr
 *  - *arr is not nullptr
 * - -2: Memory error
 *  - Memory allocation failed for arr (no values changed)
 *  - Memory reallocation failed for list (values changed)
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n - m) amortized
 * - worst case: O(n - m) amortized
 */
int list_split_arr_int(list_int *list, int index, int **arr, unsigned int *size);

/**
 * makes a list_int frozen
 *
 * if a list_int is frozen, the values inside can be freely modified, but the capacity cannot be edited.
 * functions will not automatically resize the list, and instead will add values until it is at capacity
 * any values that my end up outside the range of the list's capacity will be lost/overwritten,
 * any function that directly changes list->capacity will return an error (-3)
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * // my_list->capacity = 10
 * list_freeze_int(my_list);
 * // my_list->frozen = true
 * int state;
 * for (int i = 0; i < 10; i++)
 * {
 *     state = list_append_int(my_list, i);
 * }
 * // my_list = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5}
 * // state = 0 no error value returned
 *
 * state = list_insert_int(my_list, 4, 10);
 * // my_list = {1, 2, 3, 4, 10, 5, 1, 2, 3, 4} (5 was "pushed" from the list)
 * // state = 0
 * state = list_clear_int(list);
 * // my_list = {} now the data is cleared, but the list still has the capacity for 10 int
 * // my_list->capacity = 10
 * // my_list->size = 0
 * // state = 0
 * state = list_resize_int(list, 5);
 * `Error: Attempt to resize a frozen list.`
 * // state = -3
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
void list_freeze_int(list_int *list);

/**
 * unfreezes a list
 *
 *
 * Example:
 * @code
 * list_unfreeze_int(my_list);
 * // my_list->frozen = false
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 *
 * @note see list_freeze_int() for list->frozen documentation

 */
void list_unfreeze_int(list_int *list);

/**
 * makes a list_int readonly
 *
 * if a list_int is readonly, no modifications to the list are allowed and will return an error (-3)
 * the exception is list_destroy_int(), which will still destroy and free the list
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * // my_list->capacity = 10
 * list_readonly_int(my_list);
 * // my_list->readonly = true
 * int state;
 * state = list_append_int(my_list, 0);
 * `Error: Attempt to edit readonly list`
 * // my_list = {1, 2, 3, 4, 5}
 * // state = -3
 * state = list_destroy_int(&my_list);
 * // state = 0 (my_list is freed)
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
void list_readonly_int(list_int *list);

/**
 * makes a list_int writable
 *
 * Example:
 * @code
 * list_writable_int(my_list);
 * // my_list->readonly = false
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 *
 * @note see list_readonly_int() for list->readonly documentation
 */
void list_writable_int(list_int *list);

/**
 * makes a list_int static
 *
 * if a list_int is static, no modifications to the list are allowed and will return an error (-3)
 * this also means that the list cannot be destroyed
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * // my_list->capacity = 10
 * list_static_int(my_list);
 * // my_list->_static = true
 * int state;
 * state = list_append_int(my_list, 0);
 * `Error: Attempt to edit static list.`
 * // my_list = {1, 2, 3, 4, 5}
 * // state = -3
 * state = list_destroy_int(&my_list);
 * `Error: Attempt to destroy a static list.`
 * // state = -3 (my_list still exists and is not freed!)
 * list_mutable_int(my_list);
 * state = list_destroy_int(&my_list);
 * // state = 0 (my_list was freed)
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 *
 * @note carefully using this, while this may be usefully to ensure a list does not get destroyed when it doesn't need to in a sensitive codebase,
 * improper handling of this can result in careless memory leaks
 */
void list_static_int(list_int *list);

/**
 * makes a list_int mutable/not static
 *
 * Example:
 * @code
 * list_mutable_int(my_list);
 * // my_list->_static = false
 * int state = list_destroy_int(&my_list);
 * // state = 0 (my_list was freed)
 * @endcode
 *
 * @param list a pointer to a list_int
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 *
 * @note see list_static_int() for list->_static documentation
 */
void list_mutable_int(list_int *list);


/**
 * destroys a list_int, freeing memory and assigns list to nullptr
 *
 * Example:
 * @code
 * list_destroy_int(&my_int_list); // memory freed safely
 * @endcode
 *
 * @param list address to the pointer of a list_int
 *
 * @return success state:
 * - 0: destroyed list
 * - -3: List mutability error
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
int list_destroy_int(list_int **list);


/**
 * clears the data in a list and resizes it to 1
 *
 * Example:
 * @code
 * // my_list->size 2
 * // my_list->capacity 5
 * list_clear_int(my_list);
 * // my_list->size 0
 * // my_list->capacity 1
 * @endcode
 *
 * @param list pointer to a list_int
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error
 *  - Memory reallocation failure in function list_resize_int
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
int list_clear_int(list_int *list);


/**
 * reverses the data order
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * list_reverse_int(my_list);
 * // my_list = {5, 4, 3, 2, 1}
 * @endcode
 *
 * @param list pointer to a list_int
 *
 * @return success state
 * - 0: Function executed successfully
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n/2)
 * - worst case: O(n/2)
 */
int list_reverse_int(list_int *list);


/**
 * sorts list from least to greatest
 *
 * Example:
 * @code
 * // my_list = {3, 1, 5, 2, 4}
 * list_sort_int(my_list);
 * // my_list = {1, 2, 3, 4, 5}
 * @endcode
 *
 * @param list pointer to a list_int
 *
 * @return success state
* - 0: Function executed successfully
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n log n)
 * - worst case: O(n log n)
 */
int list_sort_int(list_int *list);


/**
 * finds the maximum value and the first occurrence of it in a list_int, or -1 index if list->size = 0
 *
 * Example:
 * @code
 * int val;
 * int index;
 * // my_list =  {2, 6, 2, 4, 7, 7, 2}
 * list_max_int(my_list, &val, &index);
 * // val = 7
 * // index = 4
 * @endcode
 *
 * @param list pointer to a list_int
 * @param return_val address for the returned value
 * @param return_index address for the returned index
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - list->size = 0
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_max_int(list_int *list, int *return_val, long *return_index);


/**
 * finds the minimum value and the first occurrence of it in a list_int, or -1 index if list->size = 0
 *
 * Example:
 * @code
 * int val;
 * int index;
 * // my_list = {2, 6, 2, 4, 7, 7, 2}
 * list_min_int(my_list, &val, &index);
 * // val = 2
 * // index = 0
 * @endcode
 *
 * @param list pointer to a list_int
 * @param return_val address for the returned value
 * @param return_index address for the returned index
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - list->size = 0
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_min_int(list_int *list, int *return_val, long *return_index);

/**
 * finds the median value in a list_int, or 0 if list->size = 0
 * sorts list if not already sorted
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5, 10}
 * float val;
 * list_median_int(my_list, &val);
 * // val = 3.5
 * @endcode
 *
 * @params list pointer to a list_int
 * @params return_val address for the returned value
 *
 * @return success state
 * - 0: Function executed successfully
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n log n)
 *  - if list is sorted
 * - worst case: O(n^2)
 *  - if list is not sorted
 */
int list_median_int(list_int *list, float *return_val);

/**
 * finds the median of a list_int
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5, 10}
 * list_mode_int(my_list, &val, &amount, true);
 * // val = 2
 * // amount = 1
 * @endcode
 *
 * @param list pointer to a list_int
 * @param return_val address for the returned value
 * @param return_amount address for the amount of the mode
 * @param sort if the list should be sorted before finding the mode, if false, attempts to create a sorted copy, if memory allocation fails, uses a less efficient method
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 * - -2: Memory error
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n log n)
 *  - if list is sorted
 * - worst case: O(n^2)
 *  - if list is not sorted and fails to make a sorted copy
 */
int list_mode_int(list_int *list, int *return_val, unsigned int *return_amount, bool sort);

/**
 * finds the range of a list_int
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * int val;
 * list_range_int(my_list, &val);
 * // val = 4
 * @endcode
 *
 * @param list pointer to a list_int
 * @param return_val address for the returned value
 *
 *
 * @return success state
 * - 0: Function executed successfully
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_range_int(list_int *list, int *return_val);

/**
 * Appends a value to the end of the list.
 *
 * Example:
 * @code
 * list_int *list = init_list_int(10);
 * if (list == nullptr) return -1;
 * for (int i = 0; i < 10; i++) {
 *     list_append_int(list, i);
 * }
 * list_debug_int(list);
 * list_destroy_int(list);
 * @endcode
 *
 * @param list pointer to a list_int
 * @param value the value to append
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (value not added)
 *  - list->capacity is at the uint limit
 *  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1) amortized
 * - worst case: O(1) amortized
 */
int list_append_int(list_int *list, int value);


/**
 * Inserts a value at the specified index, doubles the capacity if there is no more space left.
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * list_insert_int(my_list, 2, 10);
 * // my_list = {1, 2, 10, 3, 4, 5}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param index the index to insert the value at, must be >= 0
 * @param value the value to insert
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 * - -2: Memory error (value not added) 
 *  - list->capacity is at the uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(n)
 */
int list_insert_int(list_int *list, unsigned int index, int value);


/**
 * Gets the value at the specified index.
 *
 * Example:
 * @code
 * int value;
 * // my_list = {1, 6, 5, 1, 3}
 * list_get_int(my_list, 2, &value);
 * // value = 5
 * @endcode
 *
 * @param list pointer to a list_int
 * @param index index of the value
 * @param value address to assign the value to
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
int list_get_int(list_int *list, unsigned int index, int *value);


/**
 * Sets the index of the data to the specified value.
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * list_set_int(my_list, 2, 10);
 * // my_list = {1, 2, 10, 4, 5}
 * @endcode
 *
 * @param list pointer to list_int
 * @param index index to set
 * @param value value to set the specified index to
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds

* - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(1)
 */
int list_set_int(list_int *list, unsigned int index, int value);


/**
 * Removes the last value from a list.
 * Resizes the list by 1/2 if size < list->capacity / 4
 * Example:
 * @code
 * int value;
 * // my_list = {1, 2, 3, 4}
 * // my_list->capacity = 16
 * list_pop_back_int(my_list, &value);
 * // my_list = {1, 2, 3}
 * // my_list->capacity = 8
 * // value = 4
 * @endcode
 *
 * @param list pointer to a list_int, list must contain >= 1 elements
 * @param return_val address for the return value to be assigned to
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - list->size = 0
 * - -2: Memory error (value is removed)
 *  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1) amortized
 * - worst case: O(1) amortized
 */
int list_pop_back_int(list_int *list, int *return_val);


/**
 * Removes the value at the specified index, shifts the values at the right of the index
 * Resizes list if size < capacity / 4.
 *
 * Example:
 * @code
 * // my_list = {1, 2, 3, 4, 5}
 * list_remove_at_int(my_list, 2);
 * // my_list = {1, 2, 4, 5}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param index index to be removed, must be greater than 0
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 * - -2: Memory error (value is removed)
 *  - list->capacity is at the uint limit
 *  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(n)
 */
int list_remove_at_int(list_int *list, unsigned int index);


/**
 * Removes the first instance of value.
 *
 * Example:
 * @code
 * // my_list = {1, 2, 10, 8, 10, 7}
 * list_remove_int(my_list, 10);
 * // my_list = {1, 2, 8, 10, 7}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param value value to remove
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (value is removed)
*  - Memory reallocation failure
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_remove_int(list_int *list, int value);


/**
 * Removes every instance of target.
 *
 * Example:
 * @code
 * // my_list = {1, 2, 10, 8, 10, 7}
 * list_remove_all_int(my_list, 10);
 * // my_list = {1, 2, 8, 7}
 * @endcode
 *
 * @param list pointer to list_int
 * @param target value to remove
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (values are removed)
*  - Memory reallocation failure
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n^2)
 */
int list_remove_all_int(list_int *list, int target);


/**
 * Removes the values in the index range (start_index, end_index].
 *
 * Example:
 * @code
 * // my_list = {1, 2, 10, 8, 10, 7}
 * list_remove_range(my_list, 2, 4);
 * // my_list = {1, 2, 7}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param start_index start index, must be >= 0 and < end_index
 * @param end_index the end-excluded index, must be > start_index
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 *  - start_index >= end_index
 * - -2: Memory error (values are removed)
*  - Memory reallocation failure
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n^2)
 */
int list_remove_range_int(list_int *list, unsigned int start_index, unsigned int end_index);


/**
 * Gets and removes the value at the specified index, shifts the values at the right of the index to the left, resizes list if size < capacity / 4.
 *
 * Example:
 * @code
 * int value;
 * // my_list = {1, 2, 10, 8, 10, 7}
 * list_take_int(my_list, 3, &value);
 * // value = 8
 * // my_list = {1, 2, 10, 10, 7}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param index index of the value, must be >= 0
 * @param value address to assign the value to
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of range
* - -2: Memory error (value is removed)
*  - Memory reallocation failure
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(n)
 */
int list_take_int(list_int *list, unsigned int index, int *value);


/**
 * Resizes the list to the number of elements it contains.
 *
 * Example:
 * @code
 * // my_list->capacity = 8
 * // my_list->size = 3
 * list_shrink_int(my_list);
 * // my_list->capacity = 3
 * // my_list->size = 3
 * @endcode
 *
 * @param list pointer to a list_int
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (no change occurs)
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is frozen
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_shrink_int(list_int *list);


/**
 * Resizes the list capacity to the specified size.
 *
 * Example:
 * @code
 * // list->capacity = 10
 * // list->size = 7
 * list_resize_int(my_list, 20);
 * // list->capacity = 20
 * // list_size = 7
 * @endcode
 *
 * @param list pointer to a list_int
 * @param new_size new capacity of the list, if 0 sets list->capacity = 1 and list->size = 0
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (no change occurs)
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is frozen
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1) amortized
 * - worst case: O(1) amortized
 */
int list_resize_int(list_int *list, unsigned int new_size);


/**
 * Assigns an array containing the values of the list, note that the array is allocated memory for the values.
 *
 * Example:
 * @code
 * int *arr;
 * unsigned int size;
 * // my_list = {1, 2, 3, 4, 5}
 * list_to_arr_int(my_list, &arr, &size);
 * // arr = {1, 2, 3, 4, 5}
 * // size = 5
 * @endcode
 *
 * @param list pointer to a list_int
 * @param arr address to an int pointer to assign values and allocate memory to, *arr must be a nullptr
 * @param size address to assign the size of the array to
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - &arr = nullptr
 *  - arr != nullptr
 * - -2: Memory error
 *  - Failed memory allocation for *arr
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_to_arr_int(list_int *list, int **arr, unsigned int *size);


/**
 * Replaces list->data values with an int array's values.
 *
 * Example:
 * @code
 * // my_list = {6, 6, 1 ,8, 1}
 * int arr[3] = {1, 2, 3};
 * list_reset_int(my_list, arr, 3);
 * // my_list = {1, 2, 3}
 * @endcode
 *
 * @param list pointer to list_int
 * @param arr array to move into list->data, cannot be nullptr
 * @param size size of the array, must be > 0 to prevent undefined behavior for memory allocation
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - arr = nullptr
 * - -2: Memory error (no change occurs)
 *  - Failed memory reallocation
*
 * - -3: List mutability error
 *  - list is frozen and arr size != list->capacity
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(1) amortized
 * - worst case: O(1) amortized
 */
int list_reset_int(list_int *list, int *arr, unsigned int size);


/**
 * Writes data directly to the specified index, overriding any data it overlaps.
 *
 * Example:
 * @code
 * int arr[3] = {1, 2, 3};
 * // my_list = {3, 8, 2, 4, 1, 9}
 * list_write_arr_int(my_list, arr, 3, 2);
 * // my_list = {3, 8, 1, 2, 3, 9}
 * @endcode
 *
 * @param list pointer to list_int
 * @param arr arr of int, cannot be nullptr
 * @param size size of arr
 * @param index starting index to write, cannot be > list->size
 *
 * @return success state
 * - 0: Successfully overwrote data
 * - -1: Parameter error
 *  - arr = nullptr
 *  - Index out of bounds
 * - -2: Memory error (values not added)
 *  - list->capacity + added data would exceed uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(m) amortized
 * - worst case: O(m) amortized
 */
int list_write_arr_int(list_int *list, int *arr, unsigned int size, unsigned int index);


/**
 * Finds and returns the index of the first instance of target value, or -1 if value is not found in list.
 *
 * Example:
 * @code
 * long index;
 * // my_list = {1, 7, 3, 10, 8, 2}
 * list_contains_int(my_list, 10, &index, 0);
 * // index = 3
 * @endcode
 *
 * @param list pointer to a list_int
 * @param target the int you are trying to find
 * @param return_index address for the return value
 * @param start_index the index to start at
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 *
 * @note complexity:
 * - best case: O(1)
 * - worst case: O(n)
 */
int list_contains_int(list_int *list, int target, long *return_index, unsigned int start_index);


/**
 * Finds a specified sequence in a list and returns that index, or -1 if sequence not found.
 *
 * Example:
 * @code
 * int arr[3] = {1, 2, 3};
 * long index;
 * // my_list = {5, 6, 1, 7, 1, 2, 3, 8}
 * list_contains_seq_int(my_list, arr, 3, &index, 0);
 * // index = 4
 * @endcode
 *
 * @param list pointer to a list_int
 * @param arr pointer to int array
 * @param size the size of arr
 * @param return_index the address of the return value
 * @param start_index the index to start search at
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 *  - arr = nullptr
 *
 * @note complexity:
 * - best case: O(m)
 * - worst case: O((n - m) * m)
 */
int list_contains_seq_int(list_int *list, int *arr, unsigned int size, long *return_index, unsigned int start_index);


/**
 * Appends a pointer array to a list.
 *
 * Example:
 * @code
 * int arr[3] = {1, 2, 3};
 * // my_list = {1, 2, 3, 4, 5}
 * list_append_arr_int(my_list, arr, 3);
 * // my_list = {1, 2, 3, 4, 5, 1, 2, 3}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param arr array, must be greater than 0
 * @param size size of arr
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - arr = nullptr
 * - -2: Memory error (values not added)
 *  - list->capacity + added data would exceed uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(m) amortized
 * - worst case: O(m) amortized
 */
int list_append_arr_int(list_int *list, int *arr, unsigned int size);


/**
 * Inserts an array into a list.
 *
 * Example:
 * @code
 * int arr[3] = {1, 2, 3};
 * // my_list = {1, 2, 3, 4, 5}
 * list_insert_arr_int(my_list, arr, 3, 2);
 * // my_list = {1, 2, 1, 2, 3, 3, 4, 5}
 * @endcode
 *
 * @param list pointer to a list_int
 * @param arr array, must be greater than 0
 * @param size size of arr
 * @param index the index to start the insertion at
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - arr = nullptr
 *  - Index out of bounds
 * - -2: Memory error (values not added)
 *  - list->capacity + added data would exceed uint limit
 *  - Failed memory reallocation
*
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 * @note complexity:
 * - best case: O(m) amortized
 * - worst case: O(n + m) amortized
 */
int list_insert_arr_int(list_int *list, int *arr, unsigned int size, unsigned int index);


/**
 * Appends the data of list2 onto list1.
 *
 * Example:
 * @code
 * // my_other_list = {1, 2, 3}
 * // my_list = {1, 2, 3, 4, 5}
 * list_append_list_int(my_list, my_other_list);
 * // my_list = {1, 2, 3, 4, 5, 1, 2, 3}

 * @endcode
 *
 * @param list1 pointer to destination list
 * @param list2 pointer to target list
 *
 * @return success state
 * - 0: Function executed successfully
 * - -2: Memory error (values not added)
 *  - list1->capacity + added data would exceed uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(m) amortized
 * - worst case: O(m) amortized
 */
int list_append_list_int(list_int *list1, list_int *list2);


/**
 * Inserts a list into a list.
 *
 * Example:
 * @code
 * list_insert_list_int(list1, list2, 2);
 * // my_other_list = {1, 2, 3}
 * // my_list = {1, 2, 3, 4, 5}
 * list_insert_arr_int(my_list, my_other_list, 2);
 * // my_list = {1, 2, 1, 2, 3, 3, 4, 5}
 * @endcode
 *
 * @param list1 pointer to a list_int
 * @param list2 pointer to list_int to be inserted
 * @param index the index to start the insertion at
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 * - -2: Memory error (values not added)
 *  - list1->capacity + added data would exceed uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(m)
 * - worst case: O(n + m)
 */
int list_insert_list_int(list_int *list1, list_int *list2, unsigned int index);


/**
 * Finds a specified sub-sequence (list2) in list1 and returns that index, or -1 if sequence not found.
 *
 * Example:
 * @code
 * long index;
 * list_contains_list_int(list1, list2, &index, 0);
 * // my_other_list = {1, 2, 3}
 * long index;
 * // my_list = {5, 6, 1, 7, 1, 2, 3, 8}
 * list_contains_list_int(my_list, my_other_list, &index, 0);
 * // index = 4
 * // index now contains the index of the first instance of the sub-sequence, or -1 if not found
 * @endcode
 *
 * @param list1 pointer to a list_int
 * @param list2 pointer to a list_int
 * @param return_index the address of the return value
 * @param index the index to start search at
 *
 * @return success state
 * - 0: Function executed successfully
 * - -1: Parameter error
 *  - Index out of bounds
 * - -2: Memory error (values not added)
 *  - list1->capacity + added data would exceed uint limit
*  - Failed memory reallocation
 * - -3: List mutability error
 *  - list is readonly
 *  - list is static
 *
 * @note complexity:
 * - best case: O(m)
 * - worst case: O((n - m) * m)
 */
int list_contains_list_int(list_int *list1, list_int *list2, long *return_index, unsigned int index);


/**
 * Gets the sum of every value in a list.
 *
 * Example:
 * @code
 * list_int *my_list = init_list_int(5);
 * long sum;
 * list_sum_int(my_list, &sum);
 * @endcode
 *
 * @param list pointer to a list_int
 * @param value address of a long to assign the value to
 *
 * @return success state
 * - 0: Function executed successfully
 *
 * @note complexity:
 * - best case: O(n)
 * - worst case: O(n)
 */
int list_sum_int(list_int *list, long *value);


/**
 * Gets the sum of every value in two lists.
 *
 * Example:
 * @code
 * list_int *list1 = init_list_int(5);
 * list_int *list2 = init_list_int(5);
 * long long sum;
 * list_sum_list_int(list1, list2, &sum);
 * @endcode
 *
 * @param list1 pointer to a list_int
 * @param list2 pointer to a list_int
 * @param value address of a long long to assign the value to
 *
 * @return success state
 * - 0: Function executed successfully
 *
 * @note complexity:
 * - best case: O(n + m)
 * - worst case: O(n + m)
 */
int list_sum_list_int(list_int *list1, list_int *list2, long long *value);

void list_print_int(list_int *list);

#endif // LIST_H
