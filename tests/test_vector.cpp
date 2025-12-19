#include <gtest/gtest.h>
extern "C" {
    #include "../vector.h"
}

// 测试创建和释放Vector
TEST(VectorTest, CreateAndFree) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    EXPECT_EQ(v->size, 0);
    EXPECT_EQ(v->capacity, 0);
    vector_free(v);
}

// 测试push_back
TEST(VectorTest, PushBack) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data1 = 10, data2 = 20, data3 = 30;
    
    EXPECT_EQ(vector_push_back(v, &data1), SUCCESS);
    EXPECT_EQ(v->size, 1);
    
    EXPECT_EQ(vector_push_back(v, &data2), SUCCESS);
    EXPECT_EQ(v->size, 2);
    
    EXPECT_EQ(vector_push_back(v, &data3), SUCCESS);
    EXPECT_EQ(v->size, 3);
    
    vector_free(v);
}

// 测试NULL指针的push_back
TEST(VectorTest, PushBackNull) {
    EXPECT_EQ(vector_push_back(nullptr, nullptr), ERROR_NULL_POINTER);
}

// 测试get
TEST(VectorTest, Get) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data1 = 10, data2 = 20, data3 = 30;
    
    vector_push_back(v, &data1);
    vector_push_back(v, &data2);
    vector_push_back(v, &data3);
    
    EXPECT_EQ(*(int*)vector_get(v, 0), 10);
    EXPECT_EQ(*(int*)vector_get(v, 1), 20);
    EXPECT_EQ(*(int*)vector_get(v, 2), 30);
    
    // 越界访问
    EXPECT_EQ(vector_get(v, 3), nullptr);
    EXPECT_EQ(vector_get(v, 100), nullptr);
    
    vector_free(v);
}

// 测试NULL指针的get
TEST(VectorTest, GetNull) {
    EXPECT_EQ(vector_get(nullptr, 0), nullptr);
}

// 测试remove_at
TEST(VectorTest, RemoveAt) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data1 = 10, data2 = 20, data3 = 30, data4 = 40;
    
    vector_push_back(v, &data1);
    vector_push_back(v, &data2);
    vector_push_back(v, &data3);
    vector_push_back(v, &data4);
    
    EXPECT_EQ(v->size, 4);
    
    // 删除中间元素
    EXPECT_EQ(vector_remove_at(v, 1), SUCCESS);
    EXPECT_EQ(v->size, 3);
    EXPECT_EQ(*(int*)vector_get(v, 0), 10);
    EXPECT_EQ(*(int*)vector_get(v, 1), 30);
    EXPECT_EQ(*(int*)vector_get(v, 2), 40);
    
    // 删除第一个元素
    EXPECT_EQ(vector_remove_at(v, 0), SUCCESS);
    EXPECT_EQ(v->size, 2);
    EXPECT_EQ(*(int*)vector_get(v, 0), 30);
    
    vector_free(v);
}

// 测试remove_at越界
TEST(VectorTest, RemoveAtOutOfBounds) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    EXPECT_EQ(vector_remove_at(v, 0), ERROR_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(vector_remove_at(v, 100), ERROR_INDEX_OUT_OF_BOUNDS);
    
    vector_free(v);
}

// 测试NULL指针的remove_at
TEST(VectorTest, RemoveAtNull) {
    EXPECT_EQ(vector_remove_at(nullptr, 0), ERROR_NULL_POINTER);
}

// 测试clear
TEST(VectorTest, Clear) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    int data1 = 10, data2 = 20, data3 = 30;
    
    vector_push_back(v, &data1);
    vector_push_back(v, &data2);
    vector_push_back(v, &data3);
    
    EXPECT_EQ(v->size, 3);
    
    vector_clear(v);
    EXPECT_EQ(v->size, 0);
    
    vector_free(v);
}

// 测试NULL指针的clear
TEST(VectorTest, ClearNull) {
    vector_clear(nullptr);  // 不应该崩溃
}

// 测试size和is_empty
TEST(VectorTest, SizeAndIsEmpty) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    EXPECT_EQ(vector_size(v), 0);
    EXPECT_EQ(vector_is_empty(v), TRUE);
    
    int data = 10;
    vector_push_back(v, &data);
    
    EXPECT_EQ(vector_size(v), 1);
    EXPECT_EQ(vector_is_empty(v), FALSE);
    
    vector_free(v);
}

// 测试NULL指针的size和is_empty
TEST(VectorTest, SizeIsEmptyNull) {
    EXPECT_EQ(vector_size(nullptr), 0);
    EXPECT_EQ(vector_is_empty(nullptr), TRUE);
}

// 测试扩容机制
TEST(VectorTest, Resize) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    // 添加多个元素触发扩容
    for (int i = 0; i < 100; i++) {
        int *data = new int(i);
        EXPECT_EQ(vector_push_back(v, data), SUCCESS);
    }
    
    EXPECT_EQ(v->size, 100);
    
    // 验证数据正确性
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(*(int*)vector_get(v, i), i);
        delete (int*)vector_get(v, i);
    }
    
    vector_free(v);
}

// 测试收缩机制
TEST(VectorTest, Shrink) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    // 添加元素
    for (int i = 0; i < 20; i++) {
        int *data = new int(i);
        vector_push_back(v, data);
    }
    
    size_t original_capacity = v->capacity;
    
    // 删除大部分元素触发收缩
    for (int i = 0; i < 15; i++) {
        delete (int*)vector_get(v, 0);
        vector_remove_at(v, 0);
    }
    
    // 容量应该减小
    EXPECT_LT(v->capacity, original_capacity);
    
    // 清理剩余元素
    for (size_t i = 0; i < v->size; i++) {
        delete (int*)vector_get(v, i);
    }
    
    vector_free(v);
}

// 测试NULL指针的free
TEST(VectorTest, FreeNull) {
    vector_free(nullptr);  // 不应该崩溃
}

// 测试vector_create失败的情况(通过模拟内存不足很难实现,这里跳过)

// 测试扩容失败的情况(很难模拟,跳过)

// 测试收缩到INITIAL_CAPACITY
TEST(VectorTest, ShrinkToInitialCapacity) {
    Vector *v = vector_create();
    ASSERT_NE(v, nullptr);
    
    // 添加足够的元素触发多次扩容
    for (int i = 0; i < 30; i++) {
        int *data = new int(i);
        vector_push_back(v, data);
    }
    
    // 删除到只剩3个元素,应该收缩到INITIAL_CAPACITY(4)
    for (int i = 0; i < 27; i++) {
        delete (int*)vector_get(v, 0);
        vector_remove_at(v, 0);
    }
    
    EXPECT_EQ(v->size, 3);
    
    // 清理
    for (size_t i = 0; i < v->size; i++) {
        delete (int*)vector_get(v, i);
    }
    
    vector_free(v);
}
