#include "vector.h"
#include <stdlib.h>
#include <string.h>

/* 初始容量 */
#define INITIAL_CAPACITY 4

/* 内部辅助函数:扩容 */
static ErrorCode vector_resize(Vector *self, size_t new_capacity) {
    if (self == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    void **new_data = (void **)realloc(self->data, sizeof(void *) * new_capacity);
    if (new_data == NULL) {
        return ERROR_OUT_OF_MEMORY;
    }
    
    self->data = new_data;
    self->capacity = new_capacity;
    return SUCCESS;
}

/* 构造函数：创建一个新的空向量 */
Vector *vector_create(void) {
    Vector *v = (Vector *)malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }
    
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    
    return v;
}

/* 添加元素到末尾 */
ErrorCode vector_push_back(Vector *v, void *element) {
    if (v == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    /* 检查是否需要扩容 */
    if (v->size >= v->capacity) {
        /* 几何级数扩容: 容量为0时初始化为4,否则翻倍 */
        size_t new_capacity = (v->capacity == 0) ? INITIAL_CAPACITY : v->capacity * 2;
        ErrorCode err = vector_resize(v, new_capacity);
        if (err != SUCCESS) {
            return err;
        }
    }
    
    v->data[v->size++] = element;
    return SUCCESS;
}

/* 获取指定索引的元素 */
void *vector_get(const Vector *v, size_t index) {
    if (v == NULL || index >= v->size) {
        return NULL;
    }
    
    return v->data[index];
}

/* 删除指定索引的元素 */
ErrorCode vector_remove_at(Vector *v, size_t index) {
    if (v == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (index >= v->size) {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    /* 将后面的元素前移 */
    for (size_t i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    
    v->size--;
    v->data[v->size] = NULL; /* FIX: 清除对已删除元素的引用，防止野指针误用 */
    
    /* 收缩策略: 当size < capacity/4时,自动收缩内存 */
    if (v->size > 0 && v->size < v->capacity / 4) {
        size_t new_capacity = v->capacity / 2;
        if (new_capacity < INITIAL_CAPACITY) {
            new_capacity = INITIAL_CAPACITY;
        }
        vector_resize(v, new_capacity);
    }
    
    return SUCCESS;
}

/* 清空所有元素 */
void vector_clear(Vector *v) {
    if (v != NULL) {
        v->size = 0;
    }
}

/* 释放vector */
void vector_free(Vector *v) {
    if (v != NULL) {
        if (v->data != NULL) {
            free(v->data);
            v->data = NULL;
        }
        free(v);
    }
}

/* 获取当前元素个数 */
size_t vector_size(const Vector *v) {
    if (v == NULL) {
        return 0;
    }
    return v->size;
}

/* 判断是否为空 */
Bool vector_is_empty(const Vector *v) {
    if (v == NULL) {
        return TRUE;
    }
    return (v->size == 0) ? TRUE : FALSE;
}