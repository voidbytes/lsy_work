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

/* 构造函数 */
Vector *vector_create(void) {
    Vector *v = (Vector *)malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }
    
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    
    /* 绑定成员方法 */
    v->push_back = vector_push_back;
    v->get = vector_get;
    v->remove_at = vector_remove_at;
    v->clear = vector_clear;
    v->free = vector_free;
    
    return v;
}

/* 添加元素到末尾 */
ErrorCode vector_push_back(Vector *self, void *element) {
    if (self == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    /* 检查是否需要扩容 */
    if (self->size >= self->capacity) {
        /* 几何级数扩容: 容量为0时初始化为4,否则翻倍 */
        size_t new_capacity = (self->capacity == 0) ? INITIAL_CAPACITY : self->capacity * 2;
        ErrorCode err = vector_resize(self, new_capacity);
        if (err != SUCCESS) {
            return err;
        }
    }
    
    self->data[self->size++] = element;
    return SUCCESS;
}

/* 获取指定索引的元素 */
void *vector_get(Vector *self, size_t index) {
    if (self == NULL || index >= self->size) {
        return NULL;
    }
    
    return self->data[index];
}

/* 删除指定索引的元素 */
ErrorCode vector_remove_at(Vector *self, size_t index) {
    if (self == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (index >= self->size) {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    /* 将后面的元素前移 */
    for (size_t i = index; i < self->size - 1; i++) {
        self->data[i] = self->data[i + 1];
    }
    
    self->size--;
    
    /* 收缩策略: 当size < capacity/4时,自动收缩内存 */
    if (self->size > 0 && self->size < self->capacity / 4) {
        size_t new_capacity = self->capacity / 2;
        if (new_capacity < INITIAL_CAPACITY) {
            new_capacity = INITIAL_CAPACITY;
        }
        vector_resize(self, new_capacity);
    }
    
    return SUCCESS;
}

/* 清空所有元素 */
void vector_clear(Vector *self) {
    if (self != NULL) {
        self->size = 0;
    }
}

/* 释放vector */
void vector_free(Vector *self) {
    if (self != NULL) {
        if (self->data != NULL) {
            free(self->data);
            self->data = NULL;
        }
        free(self);
    }
}

/* 获取当前元素个数 */
size_t vector_size(Vector *self) {
    if (self == NULL) {
        return 0;
    }
    return self->size;
}

/* 判断是否为空 */
Bool vector_is_empty(Vector *self) {
    if (self == NULL) {
        return TRUE;
    }
    return (self->size == 0) ? TRUE : FALSE;
}
