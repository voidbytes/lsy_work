#include <gtest/gtest.h>
extern "C" {
    #include "../sort.h"
    #include "../vector.h"
}

// 整数比较器
static int int_comparator(const void *a, const void *b) {
    int val1 = *(int**)a ? **(int**)a : 0;
    int val2 = *(int**)b ? **(int**)b : 0;
    return val1 - val2;
}

// 测试空Vector排序
TEST(SortTest, QuickSortNullVector) {
    quick_sort(nullptr, int_comparator);  // 不应该崩溃
}

// 测试NULL比较器
TEST(SortTest, QuickSortNullComparator) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data = 10;
    vector_push_back(v, &data);
    
    quick_sort(v, nullptr);  // 不应该崩溃
    
    vector_free(v);
}

// 测试空Vector
TEST(SortTest, QuickSortEmptyVector) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    quick_sort(v, int_comparator);  // 不应该崩溃
    EXPECT_EQ(v->size, 0);
    
    vector_free(v);
}

// 测试单元素排序
TEST(SortTest, QuickSortSingleElement) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data = 10;
    vector_push_back(v, &data);
    
    quick_sort(v, int_comparator);
    
    EXPECT_EQ(v->size, 1);
    EXPECT_EQ(*(int*)vector_get(v, 0), 10);
    
    vector_free(v);
}

// 测试多元素排序
TEST(SortTest, QuickSortMultipleElements) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        vector_push_back(v, &data[i]);
    }
    
    quick_sort(v, int_comparator);
    
    // 验证排序结果
    for (size_t i = 1; i < v->size; i++) {
        EXPECT_LE(*(int*)vector_get(v, i-1), *(int*)vector_get(v, i));
    }
    
    vector_free(v);
}

// 测试已排序数组
TEST(SortTest, QuickSortAlreadySorted) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        vector_push_back(v, &data[i]);
    }
    
    quick_sort(v, int_comparator);
    
    // 验证排序结果
    for (int i = 0; i < size; i++) {
        EXPECT_EQ(*(int*)vector_get(v, i), i + 1);
    }
    
    vector_free(v);
}

// 测试逆序数组
TEST(SortTest, QuickSortReverseSorted) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        vector_push_back(v, &data[i]);
    }
    
    quick_sort(v, int_comparator);
    
    // 验证排序结果
    for (int i = 0; i < size; i++) {
        EXPECT_EQ(*(int*)vector_get(v, i), i + 1);
    }
    
    vector_free(v);
}

// 测试重复元素
TEST(SortTest, QuickSortDuplicateElements) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data[] = {5, 2, 5, 1, 5, 3, 5, 2, 1};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        vector_push_back(v, &data[i]);
    }
    
    quick_sort(v, int_comparator);
    
    // 验证排序结果
    for (size_t i = 1; i < v->size; i++) {
        EXPECT_LE(*(int*)vector_get(v, i-1), *(int*)vector_get(v, i));
    }
    
    vector_free(v);
}

// 测试大量数据
TEST(SortTest, QuickSortLargeData) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    const int SIZE = 1000;
    int *data = new int[SIZE];
    
    // 生成随机数据
    for (int i = 0; i < SIZE; i++) {
        data[i] = SIZE - i;  // 逆序
        vector_push_back(v, &data[i]);
    }
    
    quick_sort(v, int_comparator);
    
    // 验证排序结果
    for (size_t i = 1; i < v->size; i++) {
        EXPECT_LE(*(int*)vector_get(v, i-1), *(int*)vector_get(v, i));
    }
    
    delete[] data;
    vector_free(v);
}
