#include "list_int.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void swap_int(int *a, int *b)
{
	const int temp = *a;
	*a = *b;
	*b = temp;
}


void swap_double(double *a, double *b)
{
	const double temp = *a;
	*a = *b;
	*b = temp;
}


list_int *init_list_int(unsigned int init_size)
{
	assert(init_size > 0);
	list_int *list = malloc(sizeof(list_int));
	if (list == NULL)
	{
		fprintf(stderr, "Error: Memory allocation for list failed.\n");
		return NULL;
	}

	list->data = malloc(sizeof(int) * init_size);

	if (list->data == NULL)
	{
		free(list);
		fprintf(stderr, "Error: Memory allocation for list->data failed.\n");
		return NULL;
	}
	list->capacity = init_size;
	list->size = 0;
	return list;
}


list_int *init_list_arr_int(int *arr, unsigned int size)
{
	assert(size > 0);
	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to convert NULL arr to list_int");
		return NULL;
	}

	list_int *list = init_list_int(size);
	if (list == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		return NULL;
	}

	list_write_arr_int(list, arr, size, 0);
	return list;
}


list_int *list_split_int(list_int *list, int index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->size == 0)
	{
		fprintf(stderr, "Error: Attempt to split empty list");
		return NULL;
	}

	if (list->size == 1)
	{
		fprintf(stderr, "Error: Attempt to split list with size of 1");
		return NULL;
	}

	if (index == 0)
	{
		fprintf(stderr, "Error: Attempt to split at 0");
		return NULL;
	}

	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return NULL;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return NULL;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return NULL;
	}

	unsigned int midpoint = index;

	if (index == -1)
	{
		midpoint = list->size / 2;
		if (list->size % 2 != 0)
		{
			midpoint++;
		}
	}

	const unsigned int list_new_size = midpoint;
	const unsigned int list2_size = list->size - list_new_size;

	list_int *split = init_list_int(list2_size);

	if (split == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory for new list.\n");
		return NULL;
	}

	for (long i = midpoint; i < list->size; i++)
	{
		split->data[split->size++] = list->data[i];
	}

	if (list->frozen)
	{
		list->size = list_new_size;
		return split;
	}

	list_resize_int(list, list_new_size);

	return split;

}


int list_split_arr_int(list_int *list, int index, int **arr, unsigned int *size)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->size == 0)
	{
		fprintf(stderr, "Error: Attempt to split empty list.\n");
		return -1;
	}

	if (list->size == 1)
	{
		fprintf(stderr, "Error: Attempt to split list with size of 1.\n");
		return -1;
	}

	if (index == 0)
	{
		fprintf(stderr, "Error: Attempt to split at 0.\n");
		return -1;
	}

	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}

	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to assign values to NULL address.\n");
		return -1;
	}

	if (*arr != NULL)
	{
		fprintf(stderr, "Error: Attempt to assign values to previously assigned pointer.\n");
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	unsigned short midpoint = index;

	if (index == -1)
	{
		midpoint = list->size / 2;
		if (list->size % 2 != 0)
		{
			midpoint++;
		}
	}

	const unsigned short list_new_size = midpoint;
	*size = list->size - list_new_size;

	*arr = malloc(*size * sizeof(int));

	if (*arr == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate memory for arr.\n");
		return -2;
	}

	for (unsigned int i = midpoint; i < list->size; i++)
	{
		(*arr)[list->size - i] = list->data[i];
	}

	if (list->frozen)
	{
		list->size = list_new_size;
		return 0;
	}
	list_resize_int(list, list_new_size);

	return 0;
}


void list_freeze_int(list_int *list)
{
	assert(list != NULL);
	list->frozen = true;
}

void list_unfreeze_int(list_int *list)
{
	assert(list != NULL);
	list->frozen = false;
}


void list_readonly_int(list_int *list)
{
	assert(list != NULL);
	list->readonly = true;
}


void list_writable_int(list_int *list)
{
	assert(list != NULL);
	list->readonly = false;
}


void list_static_int(list_int *list)
{
	assert(list != NULL);
	list->static_ = true;
}


void list_mutable_int(list_int *list)
{
	assert(list != NULL);
	list->static_ = false;
}


int list_destroy_int(list_int **list)
{
	if (list == NULL) return 0;
	if (*list == NULL) return 0;

	if ((*list)->static_)
	{
		fprintf(stderr, "Error: Attempt to destroy a static list.\n");
		return -3;
	}

	free((*list)->data);
	(*list)->data = NULL;
	free(*list);
	*list = NULL;
	return 0;
}


int list_clear_int(list_int *list)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->frozen)
	{
		list->size = 0;
		return 0;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (list_resize_int(list, 0) == 0)
	{
		return 0;
	}
	return -2;
}


int list_reverse_int(list_int *list)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	for (unsigned int i = 0; i < list->size / 2; i++)
	{
		swap_int(&list->data[i], &list->data[list->size - i - 1]);
	}
	return 0;
}


void merge_int(int *arr, int left, int mid, int right)
{
	const int n1 = mid - left + 1;
	const int n2 = right - mid;

	int L[n1];
	int R[n2];

	for (unsigned int i = 0; i < n1; i++)
	{
		L[i] = arr[left + i];
	}
	for (unsigned int j = 0; j < n2; j++)
	{
		R[j] = arr[mid + 1 + j];
	}

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		} else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}


void merge_sort_int(int *arr, int left, int right) // NOLINT(*-no-recursion)
{
	if (left < right)
	{
		const int mid = left + (right - left) / 2;

		merge_sort_int(arr, left, mid);
		merge_sort_int(arr, mid + 1, right);

		merge_int(arr, left, mid, right);
	}
}


int list_sort_int(list_int *list)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	merge_sort_int(list->data, 0, (int)list->size - 1);
	return 0;
}


int list_max_int(list_int *list, int *return_val, long *return_index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->size == 0)
	{
		*return_index = -1;
		return -1;
	}

	*return_val = INT_MIN;

	for (unsigned int i = 0; i < list->size; i++)
	{
		if (*return_val < list->data[i])
		{
			*return_val = list->data[i];
			*return_index = i;
		}
	}

	return 0;
}


int list_min_int(list_int *list, int *return_val, long *return_index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->size == 0)
	{
		*return_index = -1;
		return -1;
	}

	*return_val = INT_MAX;

	for (unsigned int i = 0; i < list->size; i++)
	{
		if (*return_val > list->data[i])
		{
			*return_val = list->data[i];
			*return_index = i;
		}
	}
	return 0;
}

int list_median_int(list_int *list, float *return_val)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (list->size == 0)
	{
		*return_val = 0;
		return 0;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	list_sort_int(list);
	*return_val = (float)(list->data[list->size / 2]);
	if (list->size % 2 == 0)
	{
		*return_val = (*return_val + (float)list->data[list->size / 2 - 1]) / 2.f;
	}

	return 0;
}

int list_mode_int(list_int *list, int *return_val, unsigned int *return_amount, bool sort)
{
	assert(list != NULL);
	assert(list->data != NULL);
	int *sorted = NULL;

	if (list->size == 0)
	{
		fprintf(stderr, "Error: Attempt to find values in 0 size list");
		return -1;
	}

	if (list->size == 1)
	{
		*return_val = list->data[0];
		*return_amount = 1;
		return 0;
	}

	if (sort)
	{
		const int state = list_sort_int(list);
		if (state == -3)
		{
			sort = false;
		}
		else
		{
			sorted = list->data;
		}
	}

	bool mem_allocated = false;
	int max = list->data[0], count = 0, max_count = 0;
	if (!sort)
	{
		sorted = malloc(list->size * sizeof(int));
		if (sorted)
		{
			memcpy(sorted, list->data, list->size * sizeof(int));
			sort = true;
			mem_allocated = true;
		}
		else
		{
			for (unsigned int i = 0; i < list->size; i++)
			{
				count = 0;
				const int temp = list->data[i];
				for (unsigned int k = i; k < list->size; k++)
				{
					if (list->data[k] == temp) count++;
				}
				if (count > max_count)
				{
					max_count = count;
					max = temp;
				}
			}
		}
	}

	if (sort)
	{
		for (unsigned int i = 1; i < list->size; i++)
		{
			if (sorted[i] == sorted[i - 1]) count++;
			else count = 1;
			if (count > max_count)
			{
				max_count = count;
				max = sorted[i];
			}
		}
	}

	*return_val = max;
	*return_amount = max_count;

	if (mem_allocated)
	{
		free(sorted);
		sorted = NULL;
	}
	return 0;
}

int list_range_int(list_int *list, int *return_val)
{
	assert(list != NULL);
	assert(list->data != NULL);
	*return_val = 0;
	int min = INT_MAX;
	int max = INT_MIN;
	for (unsigned int i = 0; i < list->size; i++)
	{
		if (list->data[i] < min) min = list->data[i];
		if (list->data[i] > max) max = list->data[i];
		*return_val = max - min;
	}
	return 0;
}


int list_append_int(list_int *list, int value)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (list->size >= list->capacity)
	{
		if (list->frozen) return 0;
		if (list->size == UINT_MAX)
		{
			fprintf(
				stderr,
				"Error: Max capacity of list cannot exceed %u (current max capacity: %u, attempted number of values to add: %u)",
				UINT_MAX, list->capacity, 1);
			return -2;
		}
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list, list->capacity * 2);
		if (state == -2)
		{
			fprintf(stderr, "Error: Value could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}
	list->data[list->size++] = value;
	return 0;
}


int list_insert_int(list_int *list, unsigned int index, int value)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (index > list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (list->size >= list->capacity)
	{
		if (list->frozen) return 0;
		if (list->size == UINT_MAX)
		{
			fprintf(
				stderr,
				"Error: Max capacity of list cannot exceed %u (current max capacity: %u, attempted number of values to add: %u)",
				UINT_MAX, list->capacity, 1);
			return -2;
		}
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list, list->capacity * 2);
		if (state == -2)
		{
			fprintf(stderr, "Error: Value could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}
	list->size++;
	for (long i = list->size - 1; i >= index; i--)
	{
		list->data[i + 1] = list->data[i];
	}
	list->data[index] = value;
	return 0;
}


int list_get_int(list_int *list, unsigned int index, int *value)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}
	*value = list->data[index];
	return 0;
}


int list_set_int(list_int *list, unsigned int index, int value)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	list->data[index] = value;
	return 0;
}


int list_pop_back_int(list_int *list, int *return_val)
{
	assert(list != NULL);
	assert(list->data != NULL);


	if (list->size == 0)
	{
		fprintf(stderr, "Error: Attempt to remove from 0 size list.\n");
		return 0;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	return list_take_int(list, list->size - 1, return_val);
}

int list_remove_at_int(list_int *list, unsigned int index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	for (unsigned int i = index; i < list->size - 1; i++)
	{
		list->data[i] = list->data[i + 1];
	}

	list->size--;

	if (list->size < list->capacity / 4)
	{
		if (list->frozen) return 0;
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list, list->capacity / 2);
		if (state == -2)
		{
			fprintf(stderr, "Error: Memory could not be reallocated.\n");
			return -2;
		}
	}
	return 0;
}


int list_remove_int(list_int *list, int value)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	for (unsigned int i = 0; i < list->size; i++)
	{
		if (list->data[i] == value)
		{
			return list_remove_at_int(list, i);
		}
	}
	return -1;
}


int list_remove_all_int(list_int *list, int target)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	int memory_fail = 0;
	int count = 0;
	for (unsigned int i = 0; i < list->size;)
	{
		if (list->data[i] == target)
		{
			count++;

			// ReSharper disable once CppLocalVariableMayBeConst
			int state = list_remove_at_int(list, i);
			if (state == -2)
			{
				fprintf(stderr, "Error: Memory reallocation failed.\n");
				memory_fail = -2;
			}
			continue;
		}
		i++;
	}
	return count > 0 ? (memory_fail == -2 ? -2 : 0) : -1;
}


int list_remove_range_int(list_int *list, unsigned int start_index, unsigned int end_index)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (end_index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", end_index, list->size);
		return -1;
	}

	if (start_index >= end_index)
	{
		fprintf(stderr, "Error: start_index is greater than end_index");
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	const unsigned int diff = end_index - start_index;
	for (unsigned int i = start_index; i < list->size - diff; i++)
	{
		list->data[i] = list->data[i + diff];
	}

	list->size -= diff;

	int ret_value = 0;
	while (list->size < list->capacity / 4 && list->capacity / 2 > 1)
	{
		if (list->frozen) return 0;
		if ((ret_value = list_resize_int(list, list->capacity / 2)) != 0) break;
	}
	return ret_value;
}


int list_take_int(list_int *list, unsigned int index, int *value)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index, list->size);
		return -1;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	*value = list->data[index];
	return list_remove_at_int(list, index);
}


int list_shrink_int(list_int *list)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (list->frozen)
	{
		fprintf(stderr, "Error: Attempt to resize frozen list.\n");
		return -1;
	}
	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}
	return list_resize_int(list, list->size);
}


int list_resize_int(list_int *list, unsigned int new_size)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (list->frozen)
	{
		fprintf(stderr, "Error: Attempt to resize a frozen list.\n");
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	const unsigned int true_size = new_size == 0 ? 1 : new_size;

	int *new_data = realloc(list->data, sizeof(int) * true_size);

	if (new_data == NULL)
	{
		fprintf(stderr, "Error: Memory reallocation failed.\n");
		return -2;
	}
	list->data = new_data;
	list->capacity = true_size;
	list->size = list->size >= new_size ? new_size : list->size;
	return 0;
}


int list_to_arr_int(list_int *list, int **arr, unsigned int *size)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (arr == NULL)
	{
		*size = 0;
		fprintf(stderr, "Error: Attempt to assign values to NULL arr.\n");
		return -1;
	}

	if (*arr != NULL)
	{
		fprintf(stderr, "Error: *arr is not NULL.\n");
		*arr = NULL;
	}

	if (list->size == 0)
	{
		*size = 0;
		return 0;
	}

	*arr = malloc(sizeof(int) * list->size);
	if (*arr == NULL)
	{
		fprintf(stderr, "Error: Memory allocation for arr failed.\n");
		*size = 0;
		return -2;
	}

	*size = list->size;

	for (unsigned int i = 0; i < list->size; i++)
	{
		(*arr)[i] = list->data[i];
	}

	return 0;
}


int list_reset_int(list_int *list, int *arr, unsigned int size)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to copy NULL array.\n");
		return -1;
	}

	if (list->frozen && size != list->capacity)
	{
		fprintf(stderr, "Error: Attempt to resize frozen list.\n");
		return -3;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (size == 0)
	{
		list_clear_int(list);
		return 0;
	}

	int *new_data = realloc(list->data, sizeof(int) * size);
	if (new_data == NULL)
	{
		fprintf(stderr, "Error: Memory reallocation failed.\n");
		return -2;
	}
	list->data = new_data;
	memcpy(list->data, arr, sizeof(int) * size);
	list->size = size;
	list->capacity = size;
	return 0;
}


int list_write_arr_int(list_int *list, int *arr, unsigned int size, unsigned int index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to write NULL to list.\n");
		return -1;
	}

	if (index > list->size)
	{
		fprintf(stderr, "Error: Attempt to write out of list bounds.\n");
		return -1;
	}

	if (UINT_MAX - (list->size - index) <= size)
	{
		fprintf(
			stderr,
			"Error: Max capacity of list cannot exceed %u (current max capacity: %u, attempted number of values to add %u:)",
			UINT_MAX, list->capacity, size);
		return -2;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (size == 0)
	{
		return 0;
	}

	const unsigned int needed_mem = size + index;
	unsigned int new_capacity = list->capacity;
	if (needed_mem > list->capacity && !list->frozen)
	{
		while (new_capacity <= needed_mem)
		{
			if (new_capacity > UINT_MAX / 2)
			{
				new_capacity = UINT_MAX;
				break;
			}
			new_capacity *= 2;
		}
		const int success = list_resize_int(list, new_capacity);
		if (success == -2)
		{
			fprintf(stderr, "Error: Memory reallocation failure.\n");
			return -2;
		}
	}

	for (unsigned int i = index; i < size + index && i < list->capacity; i++)
	{
		list->data[i] = arr[i - index];
	}

	list->capacity = list->frozen ? list->capacity : new_capacity;
	list->size = size + index > list->size ? (list->frozen ? list->capacity : size + index) : list->size;

	return 0;
}


int list_contains_int(list_int *list, int target, long *return_index, unsigned int start_index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (list->size == 0)
	{
		*return_index = -1;
		return 0;
	}

	if (start_index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", start_index,
				list->size);
		return -1;
	}

	for (unsigned int i = start_index; i < list->size; i++)
	{
		if (list->data[i] == target)
		{
			*return_index = i;
			return 0;
		}
	}
	*return_index = -1;
	return 0;
}


int check_seq_int(int *arr, int *seq, unsigned int seq_size, unsigned int start_index)
{
	for (unsigned int i = start_index; i < start_index + seq_size; i++)
	{
		if (seq[i - seq_size] != arr[i]) return -1;
	}
	return 0;
}


int list_contains_seq_int(list_int *list, int *arr, unsigned int size, long *return_index, unsigned int start_index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to compare NULL array.\n");
		*return_index = -1;
		return -1;
	}

	if (size == 0)
	{
		*return_index = -1;
		return 0;
	}

	if (list->size == 0)
	{
		*return_index = -1;
		return 0;
	}

	if (start_index >= list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", start_index,
				list->size);
		*return_index = -1;
		return -1;
	}

	if (size > list->size - start_index)
	{
		*return_index = -1;
		return 0;
	}

	for (unsigned int i = start_index; i < list->size - size + 1; i++)
	{
		if (list->data[i] == arr[0])
		{
			if (check_seq_int(list->data, arr, size, i) == 0)
			{
				*return_index = i;
				return 0;
			}
		}
	}
	*return_index = -1;

	return 0;
}


int list_append_arr_int(list_int *list, int *arr, unsigned int size)
{
	assert(list != NULL);
	assert(list->data != NULL);
	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to append NULL array.\n");
		return -1;
	}

	if (UINT_MAX - list->size < size)
	{
		fprintf(
			stderr,
			"Error: Max capacity of list cannot exceed %u (current max capacity %u, attempted values to add %u values)",
			UINT_MAX, list->capacity, size);
		return -2;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (size == 0)
	{
		return 0;
	}


	unsigned int orig_capacity = list->capacity;
	while (list->size + size >= orig_capacity && !list->frozen)
	{
		if (orig_capacity >= UINT_MAX / 2)
		{
			orig_capacity = UINT_MAX;
			break;
		}
		orig_capacity *= 2;
	}

	if (orig_capacity != list->capacity)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list, orig_capacity);
		if (state == -2)
		{
			fprintf(stderr, "Error: Values could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}
	for (unsigned int i = 0; i < size && i + list->size < list->capacity; i++)
	{
		list->data[i + list->size] = arr[i];
	}
	list->size += list->frozen && list->size + size > list->capacity ? list->capacity - list->size : size;
	return 0;
}


int list_insert_arr_int(list_int *list, int *arr, unsigned int size, unsigned int index)
{
	assert(list != NULL);
	assert(list->data != NULL);

	if (index > list->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\n start_index %u\nlist bounds %u", index,
				list->size);
		return -1;
	}

	if (arr == NULL)
	{
		fprintf(stderr, "Error: Attempt to append NULL array.\n");
		return -1;
	}

	if (UINT_MAX - list->size <= size)
	{
		fprintf(
			stderr,
			"Error: Max capacity of list cannot exceed %u (current max capacity %u, attempted values to add %u values)",
			UINT_MAX, list->capacity, size);
		return -2;
	}

	if (list->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (size == 0)
	{
		return 0;
	}

	unsigned int orig_capacity = list->capacity;
	while (list->size + size >= orig_capacity && !list->frozen)
	{
		if (orig_capacity >= UINT_MAX / 2)
		{
			orig_capacity = UINT_MAX;
			break;
		}
		orig_capacity *= 2;
	}

	if (orig_capacity != list->capacity)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list, orig_capacity);
		if (state == -2)
		{
			fprintf(stderr, "Error: Values could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}

	for (long i = list->size - 1; i >= index; i--)
	{
		if (i + size > list->capacity) continue;
		list->data[i + size] = list->data[i];
	}

	for (unsigned int i = 0; i < size; i++)
	{
		if (i + index > list->capacity) continue;
		list->data[index + i] = arr[i];
	}
	list->size += list->frozen && list->size + size > list->capacity ? list->capacity - list->size : size;

	return 0;
}


int list_append_list_int(list_int *list1, list_int *list2)
{
	assert(list1 != NULL);
	assert(list1->data != NULL);
	assert(list2 != NULL);
	assert(list2->data != NULL);

	if (UINT_MAX - list1->size < list2->size)
	{
		fprintf(
			stderr,
			"Error: Max capacity of list cannot exceed %u (current max capacity %u, attempted values to add %u values)",
			UINT_MAX, list1->capacity, list2->size);
		return -2;
	}

	if (list1->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list1->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	if (list2->size == 0)
	{
		return 0;
	}
	unsigned int orig_capacity = list1->capacity;
	while (list1->size + list2->size >= orig_capacity && !list1->frozen)
	{
		if (orig_capacity >= UINT_MAX / 2)
		{
			orig_capacity = UINT_MAX;
			break;
		}
		orig_capacity *= 2;
	}

	if (orig_capacity != list1->capacity)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list1, orig_capacity);
		if (state == -2)
		{
			fprintf(stderr, "Error: Values could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}

	for (unsigned int i = 0; i < list2->size && i + list1->size < list1->capacity; i++)
	{
		list1->data[i + list1->size] = list2->data[i];
	}
	list1->size += list2->size + list1->size > list1->capacity && list1->frozen
					   ? list1->capacity - list1->size
					   : list2->size;
	return 0;
}


int list_insert_list_int(list_int *list1, list_int *list2, unsigned int index)
{
	assert(list1 != NULL);
	assert(list1->data != NULL);
	assert(list2 != NULL);
	assert(list2->data != NULL);

	if (index > list1->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\n start_index %u\nlist bounds %u", index,
				list1->size);
		return -1;
	}

	if (UINT_MAX - list1->size < list2->size)
	{
		fprintf(
			stderr,
			"Error: Max capacity of list cannot exceed %u (current max capacity %u, attempted values to add %u values)",
			UINT_MAX, list1->capacity, list2->size);
		return -2;
	}

	if (list2->size == 0)
	{
		return 0;
	}

	if (list1->readonly)
	{
		fprintf(stderr, "Error: Attempt to edit readonly list.\n");
		return -3;
	}

	if (list1->static_)
	{
		fprintf(stderr, "Error: Attempt to edit static list.\n");
		return -3;
	}

	unsigned int orig_capacity = list1->capacity;
	while (list1->size + list2->size >= orig_capacity && !list1->frozen)
	{
		if (orig_capacity >= UINT_MAX / 2)
		{
			orig_capacity = UINT_MAX;
			break;
		}
		orig_capacity *= 2;
	}

	if (orig_capacity != list1->capacity)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int state = list_resize_int(list1, orig_capacity);
		if (state == -2)
		{
			fprintf(stderr, "Error: Values could not be added due to failed memory reallocation.\n");
			return -2;
		}
	}


	for (long i = list1->size - 1; i >= index; i--)
	{
		if (i + list2->size > list1->capacity) continue;
		list1->data[i + list2->size] = list1->data[i];
	}

	for (unsigned int i = 0; i < list2->size && i + index < list1->capacity; i++)
	{
		list1->data[index + i] = list2->data[i];
	}

	list1->size += list2->size + list1->size > list1->capacity && list1->frozen
					   ? list1->capacity - list1->size
					   : list2->size;

	return 0;
}


int list_contains_list_int(list_int *list1, list_int *list2, long *return_index, unsigned int index)
{
	assert(list1 != NULL);
	assert(list1->data != NULL);
	assert(list2 != NULL);
	assert(list2->data != NULL);

	if (index >= list1->size)
	{
		fprintf(stderr, "Error: Attempt to index out of bounds.\nIndex %u\nList bounds %u\n", index,
				list1->size);
		*return_index = -1;
		return -1;
	}

	if (list1->size < list2->size + index)
	{
		*return_index = -1;
		return 0;
	}

	if (list2->size == 0)
	{
		*return_index = -1;
		return 0;
	}

	for (unsigned int i = index; i < list1->size; i++)
	{
		if (list1->data[i] == list2->data[0])
		{
			if (check_seq_int(list1->data, list2->data, list2->size, i) == 0)
			{
				*return_index = i;
				return 0;
			}
		}
	}
	*return_index = -1;

	return 0;
}


int list_sum_int(list_int *list, long *value)
{
	assert(list != NULL);
	assert(list->data != NULL);

	*value = 0;

	for (unsigned int i = 0; i < list->size; i++)
	{
		*value += (long) list->data[i];
	}
	return 0;
}


int list_sum_list_int(list_int *list1, list_int *list2, long long *value)
{
	assert(list1 != NULL);
	assert(list1->data != NULL);
	assert(list2 != NULL);
	assert(list2->data != NULL);
	long value1;
	long value2;

	list_sum_int(list1, &value1);
	list_sum_int(list2, &value2);
	*value = value1 + value2;
	return 0;
}


void list_print_int(list_int *list)
{
	assert(list != NULL);
	assert(list->data != NULL);
	for (unsigned int i = 0; i < list->size; i++)
	{
		printf("%i | %i\n", i, list->data[i]);
	}
	printf("Size %u\nCapacity %u\n", list->size, list->capacity);
}
