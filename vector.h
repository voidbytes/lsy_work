#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

/* 动态数组结构体 */
typedef struct Vector {
    void **data;           /* 二级指针,存储通用元素 */
    size_t size;           /* 当前元素个数 */
    size_t capacity;       /* 当前容量 */
    
    /* 函数指针,模拟成员方法 */
    ErrorCode (*push_back)(struct Vector *self, void *element);
    void *(*get)(struct Vector *self, size_t index);
    ErrorCode (*remove_at)(struct Vector *self, size_t index);
    void (*clear)(struct Vector *self);
    void (*free)(struct Vector *self);
} Vector;

/* 构造函数 */
Vector *vector_create(void);

/* 成员方法的实际实现 */
ErrorCode vector_push_back(Vector *self, void *element);
void *vector_get(Vector *self, size_t index);
ErrorCode vector_remove_at(Vector *self, size_t index);
void vector_clear(Vector *self);
void vector_free(Vector *self);

/* 额外的工具函数 */
size_t vector_size(Vector *self);
Bool vector_is_empty(Vector *self);

#endif /* VECTOR_H */
