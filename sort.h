#ifndef SORT_H
#define SORT_H

#include "vector.h"

/* 比较器函数类型 */
typedef int (*Comparator)(const void *a, const void *b);

/* 快速排序 */
void quick_sort(Vector *v, Comparator compare);

#endif /* SORT_H */
