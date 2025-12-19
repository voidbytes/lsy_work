#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

/* 向量结构体：包含数据数组、当前大小和容量 */
typedef struct {
    void **data;           /* 存储元素的动态数组 */
    size_t size;           /* 当前元素个数 */
    size_t capacity;       /* 容量：可存储的最大元素个数 */
} Vector;

/* 构造与析构函数 */
Vector *vector_create(void);  /* 创建新的向量 */
void vector_free(Vector *v);  /* 释放向量资源 */

/* 向量操作函数 */
ErrorCode vector_push_back(Vector *v, void *element);  /* 添加元素到末尾 */
void *vector_get(const Vector *v, size_t index);        /* 获取指定索引的元素 */
ErrorCode vector_remove_at(Vector *v, size_t index);    /* 删除指定索引的元素 */
void vector_clear(Vector *v);                           /* 清空所有元素 */
size_t vector_size(const Vector *v);                    /* 获取当前元素个数 */
Bool vector_is_empty(const Vector *v);                  /* 判断向量是否为空 */

#endif /* VECTOR_H */