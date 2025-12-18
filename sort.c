#include "sort.h"

/* 交换两个元素 */
static void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

/* 分区函数 - 简化版本 */
static int partition(void **arr, int low, int high, Comparator compare) {
    void *pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        /* compare需要 void** 类型的参数 */
        if (compare(&arr[j], &pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* 快速排序递归实现 */
static void quick_sort_recursive(void **arr, int low, int high, Comparator compare) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);
        
        quick_sort_recursive(arr, low, pi - 1, compare);
        quick_sort_recursive(arr, pi + 1, high, compare);
    }
}

/* 快速排序接口 */
void quick_sort(Vector *v, Comparator compare) {
    if (v == NULL || compare == NULL || v->size <= 1) {
        return;
    }
    
    quick_sort_recursive(v->data, 0, (int)v->size - 1, compare);
}
