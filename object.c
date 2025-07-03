#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "object.h"

snek_object_t *new_snek_integer(int value)
{
    snek_object_t *object = (snek_object_t *)malloc(sizeof(snek_object_t));

    if (!object)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    object->kind = INTEGER;
    object->data.v_int = value;

    return object;
}

snek_object_t *new_snek_float(float value)
{
    snek_object_t *object = (snek_object_t *)malloc(sizeof(snek_object_t));

    if (!object)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    object->kind = FLOAT;
    object->data.v_float = value;

    return object;
}

snek_object_t *new_snek_string(char *value)
{
    snek_object_t *object = (snek_object_t *)malloc(sizeof(snek_object_t));

    if (!object)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    object->kind = STRING;
    object->data.v_string = (char *)malloc(strlen(value) + 1);

    if (!(object->data.v_string))
    {
        perror("Memory allocation failed for 'v_string'\n");
        free(object);
        return NULL;
    }

    strcpy(object->data.v_string, value);

    return object;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z)
{
    if (!x || !y || !z)
    {
        return NULL;
    }

    snek_object_t *object = (snek_object_t *)malloc(sizeof(snek_object_t));

    if (!object)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    object->kind = VECTOR3;

    object->data.v_vector3 = (snek_vector_t){
        .x = x,
        .y = y,
        .z = z,
    };

    return object;
}

snek_object_t *new_snek_array(size_t size)
{
    snek_object_t *object = (snek_object_t *)malloc(sizeof(snek_object_t));

    if (!object)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    object->kind = ARRAY;

    snek_object_t **elements = calloc(size, sizeof(snek_object_t *));

    if (!elements)
    {
        perror("couldnt allocate memory for 'elements'\n");
        free(object);
        return NULL;
    }

    snek_array_t arrayObject = {
        .size = size,
        .elements = elements,
    };

    object->data.v_array = arrayObject;

    return object;
}

bool snek_array_set(snek_object_t *snek_obj, size_t index, snek_object_t *value)
{
    if (!snek_obj || !index || value)
    {
        perror("The object or input values r NULL\n");
        return false;
    }

    if (snek_obj->kind != ARRAY)
    {
        perror("Object's kind isnt even an array\n");
        return false;
    }

    if (snek_obj->data.v_array.size <= index)
    {
        perror("The index is out of bounds\n");
        return false;
    }

    snek_obj->data.v_array.elements[index] = value;

    return true;
}

snek_object_t *snek_array_get(snek_object_t *snek_obj, size_t index)
{
    if (!snek_obj)
    {
        perror("The object is NULL \n");
        return NULL;
    }

    if (snek_obj->kind != ARRAY)
    {
        perror("Object's kind isnt even an array\n");
        return NULL;
    }

    if (snek_obj->data.v_array.size <= index)
    {
        perror("The index is out of bounds\n");
        return NULL;
    }

    return snek_obj->data.v_array.elements[index];
}

int snek_length(snek_object_t *obj)
{
    if (!obj)
    {
        perror("length 'obj' is null\n");
        return -1;
    }

    switch (obj->kind)
    {
    case INTEGER:
    case FLOAT:
        return INTFLOAT_LENGTH;

    case STRING:
        return strlen(obj->data.v_string);

    case VECTOR3:
        return VECTOR3_LENGTH;

    case ARRAY:
        return obj->data.v_array.size;

    default:
        return -1;
    }
}

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) // can be extended for subtracting, dividing, multiplying
{
    if (!a || !b)
    {
        perror("input 'a' or 'b' is null\n");
        return NULL;
    }

    switch (a->kind)
    {
    case INTEGER:

        if (b->kind == INTEGER)
        {
            return new_snek_integer(a->data.v_int + b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            return new_snek_float((float)a->data.v_int + b->data.v_float);
        }
        else
        {
            perror("Invalid INTEGER operation\n");
            return NULL;
        }

    case FLOAT:

        if (b->kind == INTEGER)
        {
            return new_snek_float(a->data.v_float + (float)b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            return new_snek_float(a->data.v_float + b->data.v_float);
        }
        else
        {
            perror("Invalid FLOAT operation\n");
            return NULL;
        }

    case STRING:

        if (b->kind != STRING)
        {
            perror("Invalid STRING operation\n");
            return NULL;
        }

        size_t length = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
        char *newString = (char *)calloc(sizeof(char *), length);
        strcat(newString, a->data.v_string);
        strcat(newString, b->data.v_string);

        snek_object_t *object = new_snek_string(newString);
        free(newString);

        return object;

    case VECTOR3:

        if (b->kind != VECTOR3)
        {
            perror("Invalid VECTOR operation\n");
            return NULL;
        }

        return new_snek_vector3(
            snek_add(a->data.v_vector3.x, b->data.v_vector3.x),
            snek_add(a->data.v_vector3.y, b->data.v_vector3.y),
            snek_add(a->data.v_vector3.z, b->data.v_vector3.z));

    case ARRAY:

        if (b->kind != ARRAY)
        {
            perror("Invalid ARRAY operation\n");
            return NULL;
        }

        size_t arrayLength = a->data.v_array.size + b->data.v_array.size;
        snek_object_t *arrayObject = new_snek_array(arrayLength);

        for (size_t i = 0; i < a->data.v_array.size; i++)
        {
            snek_array_set(arrayObject, i, snek_array_get(a, i));
        }

        for (size_t i = 0; i < b->data.v_array.size; i++)
        {
            snek_array_set(arrayObject, i + a->data.v_array.size, snek_array_get(b, i));
        }

        return arrayObject;

    default:
        perror("Invalid operation");
        return NULL;
    }
}