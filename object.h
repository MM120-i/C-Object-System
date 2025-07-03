#include <stdbool.h>
#include <stddef.h>

#pragma once

#define INTFLOAT_LENGTH 1
#define VECTOR3_LENGTH 3

typedef struct snekObject snek_object_t;

typedef struct snekVector
{
    snek_object_t *x;
    snek_object_t *y;
    snek_object_t *z;
} snek_vector_t;

typedef struct snekArray
{
    size_t size;
    snek_object_t **elements;
} snek_array_t;

typedef enum snekObjectKindt
{
    INTEGER,
    FLOAT,
    STRING,
    VECTOR3,
    ARRAY,
} snek_object_kind_t;

// Stores actual data for rach type
typedef union snekObjectDatat
{
    int v_int;
    float v_float;
    char *v_string;
    snek_vector_t v_vector3;
    snek_array_t v_array;
} snek_object_data_t;

// Generic object
typedef struct snekObject
{
    snek_object_kind_t kind;
    snek_object_data_t data;
} snek_object_t;

snek_object_t *new_snek_integer(int);
snek_object_t *new_snek_float(float);
snek_object_t *new_snek_string(char *);
snek_object_t *new_snek_vector3(snek_object_t *, snek_object_t *, snek_object_t *);
snek_object_t *new_snek_array(size_t);
bool snek_array_set(snek_object_t *, size_t, snek_object_t *);
snek_object_t *snek_array_get(snek_object_t *, size_t);
int snek_length(snek_object_t *);
snek_object_t *snek_add(snek_object_t *, snek_object_t *);
