#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>

struct Vector;

typedef struct Vector {
    int length;
    int* array;
} Vector;

Vector* InitVector();
Vector* InitVectorWithValue(int size, int value);
Vector* InitVectorArray(int size, int* array);
Vector* CopyVector(Vector* v);
void DestroyVector(Vector* v);

void swap(int* xp, int* yp);

void v_set(Vector* v, int i, int value);
int* v_get(Vector* v, int i);
Vector* v_get_btw(Vector* v, int x, int y);

int v_sum(Vector* v);
Vector* v_sum_vectors(Vector* l1, Vector* l2);

void v_append(Vector* v, int value);
void v_pop(Vector* v);
void v_insert(Vector* v, int value, int index);
void v_delete(Vector* v, int index);

void v_bubble_sort(Vector* v);
void v_sort(Vector* v);

int v_min(Vector* v);
int v_max(Vector* v);
int v_index(Vector* v, int value);
int v_count(Vector* v, int value);

int v_cmp(Vector* v1, Vector* v2); // return 1 if v1 == v2 else return 0

void v_print(Vector* v);

struct Vector2d;

typedef struct Vector2d {
    int* shape;
    int length;
    int* array;
} Vector2d;

Vector2d* Init2dVector(int w, int h, int value);
Vector2d* Copy2dVector(Vector2d* v);
void Destroy2dVector(Vector2d* v);

void v2d_set(Vector2d* v, int x, int y, int value);
int* v2d_get(Vector2d* v, int x, int y);

void v2d_set_line(Vector2d* v, int y, int* line, int line_size);
Vector* v2d_get_line(Vector2d* v, int y);

int v2d_min(Vector2d* v);
int v2d_max(Vector2d* v);
int* v2d_coords(Vector2d* v, int value);
int v2d_count(Vector2d* v, int value);

void v2d_sort(Vector2d* v);

void v2d_print(Vector2d* v);

#endif
