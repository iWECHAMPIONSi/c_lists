#include <stdio.h>
#include <stdlib.h>
#include <ranlib.h>
#include <time.h>
#include "list_int.h"


int main(void)
{
    list_int *list = init_list_int(10);
    srand(time(NULL));

    if (list == NULL) return -1;
    for (int i = 0; i < 20; i++)
    {
        list_append_int(list, rand() % 10);
    }

    list_print_int(list);

    int arr[6] = {
    1, 2, 3, 4, 5, 10
    };
    list_reset_int(list, arr, 6);
    // list_write_arr_int(list, arr, 6, 0);
    list_print_int(list);
    float val;
    list_median_int(list, &val);
    int val1;
    unsigned int amount;

    list_mode_int(list, &val1, &amount, true);
    int val2;
    list_range_int(list, &val2);
    printf("Median: %f\nMode: %i\nRange: %i\n", val, val1, val2);
    list_print_int(list);
    list_destroy_int(&list);
    return 0;
}
