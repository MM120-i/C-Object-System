#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "object.c"

// Tester function
int main(void)
{
    snek_object_t *i1 = new_snek_integer(5);
    snek_object_t *i2 = new_snek_integer(7);
    snek_object_t *f1 = new_snek_float(3.5f);

    snek_object_t *sum_int = snek_add(i1, i2);
    snek_object_t *sum_float = snek_add(i1, f1);

    printf("sum_int: %d\n", sum_int ? sum_int->data.v_int : -1);
    printf("sum_float: %f\n", sum_float ? sum_float->data.v_float : -1.0f);

    snek_object_t *s1 = new_snek_string("Hello, ");
    snek_object_t *s2 = new_snek_string("world!");
    snek_object_t *s3 = snek_add(s1, s2);

    printf("concat string: %s\n", s3 ? s3->data.v_string : "(null)");

    snek_object_t *vx = new_snek_integer(1);
    snek_object_t *vy = new_snek_integer(2);
    snek_object_t *vz = new_snek_integer(3);
    snek_object_t *v1 = new_snek_vector3(vx, vy, vz);

    snek_object_t *vx2 = new_snek_integer(4);
    snek_object_t *vy2 = new_snek_integer(5);
    snek_object_t *vz2 = new_snek_integer(6);
    snek_object_t *v2 = new_snek_vector3(vx2, vy2, vz2);

    snek_object_t *v_sum = snek_add(v1, v2);

    printf("vector sum: (%d, %d, %d)\n",
           v_sum ? v_sum->data.v_vector3.x->data.v_int : -1,
           v_sum ? v_sum->data.v_vector3.y->data.v_int : -1,
           v_sum ? v_sum->data.v_vector3.z->data.v_int : -1);

    snek_object_t *arr1 = new_snek_array(2);
    snek_object_t *arr2 = new_snek_array(2);
    snek_array_set(arr1, 0, i1);
    snek_array_set(arr1, 1, f1);
    snek_array_set(arr2, 0, s1);
    snek_array_set(arr2, 1, s2);

    snek_object_t *arr_sum = snek_add(arr1, arr2);
    printf("array sum length: %d\n", snek_length(arr_sum));

    for (size_t i = 0; i < snek_length(arr_sum); i++)
    {
        snek_object_t *elem = snek_array_get(arr_sum, i);

        if (elem)
        {
            printf("  arr_sum[%d] kind: %d\n", i, elem->kind);
        }
    }

    return 0;
}