#include <gtest/gtest.h>
#include <cstring>
extern "C" {
    #include "../model.h"
}

// 测试employee_create
TEST(EmployeeTest, CreateEmployee) {
    Employee *emp = employee_create(1001, "张三", "研发部", "2024-01-15", 22);
    ASSERT_NE(emp, nullptr);
    EXPECT_EQ(emp->id, 1001);
    EXPECT_STREQ(emp->name, "张三");
    EXPECT_STREQ(emp->department, "研发部");
    EXPECT_STREQ(emp->attend_date, "2024-01-15");
    EXPECT_EQ(emp->attend_days, 22);
    employee_free(emp);
}

// 测试employee_create NULL参数
TEST(EmployeeTest, CreateEmployeeNullParams) {
    EXPECT_EQ(employee_create(1001, nullptr, "研发部", "2024-01-15", 22), nullptr);
    EXPECT_EQ(employee_create(1001, "张三", nullptr, "2024-01-15", 22), nullptr);
    EXPECT_EQ(employee_create(1001, "张三", "研发部", nullptr, 22), nullptr);
}

// 测试employee_free
TEST(EmployeeTest, FreeEmployee) {
    Employee *emp = employee_create(1001, "张三", "研发部", "2024-01-15", 22);
    employee_free(emp);  // 不应该崩溃
    employee_free(nullptr);  // 不应该崩溃
}

// 测试employee_print
TEST(EmployeeTest, PrintEmployee) {
    Employee *emp = employee_create(1001, "张三", "研发部", "2024-01-15", 22);
    employee_print(emp);  // 不应该崩溃
    employee_print(nullptr);  // 不应该崩溃
    employee_free(emp);
}

// 测试employee_manager_create
TEST(EmployeeManagerTest, CreateManager) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    ASSERT_NE(mgr->employees, nullptr);
    EXPECT_EQ(mgr->next_id, 1001);
    EXPECT_EQ(mgr->employees->size, 0);
    employee_manager_free(mgr);
}

// 测试employee_manager_free
TEST(EmployeeManagerTest, FreeManager) {
    EmployeeManager *mgr = employee_manager_create();
    employee_manager_free(mgr);  // 不应该崩溃
    employee_manager_free(nullptr);  // 不应该崩溃
}

// 测试employee_manager_add
TEST(EmployeeManagerTest, AddEmployee) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr->employees->size, 1);
    EXPECT_EQ(mgr->next_id, 1002);
    
    err = employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr->employees->size, 2);
    EXPECT_EQ(mgr->next_id, 1003);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_add NULL参数
TEST(EmployeeManagerTest, AddEmployeeNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(employee_manager_add(nullptr, "张三", "研发部", "2024-01-15", 22), ERROR_NULL_POINTER);
    EXPECT_EQ(employee_manager_add(mgr, nullptr, "研发部", "2024-01-15", 22), ERROR_NULL_POINTER);
    EXPECT_EQ(employee_manager_add(mgr, "张三", nullptr, "2024-01-15", 22), ERROR_NULL_POINTER);
    EXPECT_EQ(employee_manager_add(mgr, "张三", "研发部", nullptr, 22), ERROR_NULL_POINTER);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_add 负数天数
TEST(EmployeeManagerTest, AddEmployeeNegativeDays) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = employee_manager_add(mgr, "张三", "研发部", "2024-01-15", -1);
    EXPECT_EQ(err, ERROR_INVALID_PARAMETER);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_remove_at
TEST(EmployeeManagerTest, RemoveAt) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    
    EXPECT_EQ(mgr->employees->size, 3);
    
    ErrorCode err = employee_manager_remove_at(mgr, 1);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr->employees->size, 2);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_remove_at 越界
TEST(EmployeeManagerTest, RemoveAtOutOfBounds) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    
    EXPECT_EQ(employee_manager_remove_at(mgr, 1), ERROR_INDEX_OUT_OF_BOUNDS);
    EXPECT_EQ(employee_manager_remove_at(mgr, 100), ERROR_INDEX_OUT_OF_BOUNDS);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_remove_at NULL
TEST(EmployeeManagerTest, RemoveAtNull) {
    EXPECT_EQ(employee_manager_remove_at(nullptr, 0), ERROR_NULL_POINTER);
}

// 测试employee_manager_remove_by_id
TEST(EmployeeManagerTest, RemoveById) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    
    ErrorCode err = employee_manager_remove_by_id(mgr, 1002);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr->employees->size, 2);
    
    err = employee_manager_remove_by_id(mgr, 9999);
    EXPECT_EQ(err, ERROR_NOT_FOUND);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_remove_by_id NULL
TEST(EmployeeManagerTest, RemoveByIdNull) {
    EXPECT_EQ(employee_manager_remove_by_id(nullptr, 1001), ERROR_NULL_POINTER);
}

// 测试employee_manager_update
TEST(EmployeeManagerTest, UpdateEmployee) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    
    ErrorCode err = employee_manager_update(mgr, 1001, "张三丰", "武当派", "2024-02-01", 28);
    EXPECT_EQ(err, SUCCESS);
    
    Employee *emp = (Employee *)mgr->employees->data[0];
    EXPECT_STREQ(emp->name, "张三丰");
    EXPECT_STREQ(emp->department, "武当派");
    EXPECT_STREQ(emp->attend_date, "2024-02-01");
    EXPECT_EQ(emp->attend_days, 28);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_update 不存在的ID
TEST(EmployeeManagerTest, UpdateEmployeeNotFound) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = employee_manager_update(mgr, 9999, "张三", "研发部", "2024-01-15", 22);
    EXPECT_EQ(err, ERROR_NOT_FOUND);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_update NULL参数
TEST(EmployeeManagerTest, UpdateEmployeeNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(employee_manager_update(nullptr, 1001, "张三", "研发部", "2024-01-15", 22), ERROR_NULL_POINTER);
    EXPECT_EQ(employee_manager_update(mgr, 1001, nullptr, "研发部", "2024-01-15", 22), ERROR_NULL_POINTER);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_update 负数天数
TEST(EmployeeManagerTest, UpdateEmployeeNegativeDays) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    
    ErrorCode err = employee_manager_update(mgr, 1001, "张三", "研发部", "2024-01-15", -5);
    EXPECT_EQ(err, ERROR_INVALID_PARAMETER);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_search 按ID
TEST(EmployeeManagerTest, SearchById) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    int id = 1001;
    Vector *results = employee_manager_search(mgr, SEARCH_BY_ID, &id);
    ASSERT_NE(results, nullptr);
    EXPECT_EQ(results->size, 1);
    
    Employee *emp = (Employee *)results->data[0];
    EXPECT_EQ(emp->id, 1001);
    
    vector_free(results);
    employee_manager_free(mgr);
}

// 测试employee_manager_search 按名字(模糊搜索)
TEST(EmployeeManagerTest, SearchByName) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "张四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "李四", "财务部", "2024-01-17", 24);
    
    const char *name = "张";
    Vector *results = employee_manager_search(mgr, SEARCH_BY_NAME, name);
    ASSERT_NE(results, nullptr);
    EXPECT_EQ(results->size, 2);
    
    vector_free(results);
    employee_manager_free(mgr);
}

// 测试employee_manager_search 按部门
TEST(EmployeeManagerTest, SearchByDepartment) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "研发部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "市场部", "2024-01-17", 24);
    
    const char *dept = "研发部";
    Vector *results = employee_manager_search(mgr, SEARCH_BY_DEPARTMENT, dept);
    ASSERT_NE(results, nullptr);
    EXPECT_EQ(results->size, 2);
    
    vector_free(results);
    employee_manager_free(mgr);
}

// 测试employee_manager_search NULL参数
TEST(EmployeeManagerTest, SearchNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    int id = 1001;
    EXPECT_EQ(employee_manager_search(nullptr, SEARCH_BY_ID, &id), nullptr);
    EXPECT_EQ(employee_manager_search(mgr, SEARCH_BY_ID, nullptr), nullptr);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort 按ID
TEST(EmployeeManagerTest, SortById) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    employee_manager_sort(mgr, SORT_BY_ID);
    
    Employee *emp1 = (Employee *)mgr->employees->data[0];
    Employee *emp2 = (Employee *)mgr->employees->data[1];
    Employee *emp3 = (Employee *)mgr->employees->data[2];
    
    EXPECT_LT(emp1->id, emp2->id);
    EXPECT_LT(emp2->id, emp3->id);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort 按名字
TEST(EmployeeManagerTest, SortByName) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    employee_manager_sort(mgr, SORT_BY_NAME);
    
    Employee *emp1 = (Employee *)mgr->employees->data[0];
    Employee *emp2 = (Employee *)mgr->employees->data[1];
    Employee *emp3 = (Employee *)mgr->employees->data[2];
    
    EXPECT_LE(strcmp(emp1->name, emp2->name), 0);
    EXPECT_LE(strcmp(emp2->name, emp3->name), 0);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort 按部门
TEST(EmployeeManagerTest, SortByDepartment) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    
    employee_manager_sort(mgr, SORT_BY_DEPARTMENT);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort 按日期
TEST(EmployeeManagerTest, SortByAttendDate) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-17", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-15", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-16", 24);
    
    employee_manager_sort(mgr, SORT_BY_ATTEND_DATE);
    
    Employee *emp1 = (Employee *)mgr->employees->data[0];
    Employee *emp2 = (Employee *)mgr->employees->data[1];
    Employee *emp3 = (Employee *)mgr->employees->data[2];
    
    EXPECT_LE(strcmp(emp1->attend_date, emp2->attend_date), 0);
    EXPECT_LE(strcmp(emp2->attend_date, emp3->attend_date), 0);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort 按出勤天数(降序)
TEST(EmployeeManagerTest, SortByAttendDays) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 25);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 20);
    
    employee_manager_sort(mgr, SORT_BY_ATTEND_DAYS);
    
    Employee *emp1 = (Employee *)mgr->employees->data[0];
    Employee *emp2 = (Employee *)mgr->employees->data[1];
    Employee *emp3 = (Employee *)mgr->employees->data[2];
    
    EXPECT_GE(emp1->attend_days, emp2->attend_days);
    EXPECT_GE(emp2->attend_days, emp3->attend_days);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_sort NULL
TEST(EmployeeManagerTest, SortNull) {
    employee_manager_sort(nullptr, SORT_BY_ID);  // 不应该崩溃
}

// 测试employee_manager_sort 无效类型
TEST(EmployeeManagerTest, SortInvalidType) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    // 使用无效的排序类型
    employee_manager_sort(mgr, (SortType)999);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_get_all
TEST(EmployeeManagerTest, GetAll) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    Vector *all = employee_manager_get_all(mgr);
    ASSERT_NE(all, nullptr);
    EXPECT_EQ(all->size, 2);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_get_all NULL
TEST(EmployeeManagerTest, GetAllNull) {
    EXPECT_EQ(employee_manager_get_all(nullptr), nullptr);
}

// 测试employee_manager_monthly_attendance
TEST(EmployeeManagerTest, MonthlyAttendance) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-02-17", 24);
    
    int total = employee_manager_monthly_attendance(mgr, "2024-01");
    EXPECT_EQ(total, 45);  // 22 + 23
    
    total = employee_manager_monthly_attendance(mgr, "2024-02");
    EXPECT_EQ(total, 24);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_monthly_attendance NULL
TEST(EmployeeManagerTest, MonthlyAttendanceNull) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(employee_manager_monthly_attendance(nullptr, "2024-01"), 0);
    EXPECT_EQ(employee_manager_monthly_attendance(mgr, nullptr), 0);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_yearly_attendance
TEST(EmployeeManagerTest, YearlyAttendance) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-02-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2023-12-17", 24);
    
    int total = employee_manager_yearly_attendance(mgr, "2024");
    EXPECT_EQ(total, 45);  // 22 + 23
    
    total = employee_manager_yearly_attendance(mgr, "2023");
    EXPECT_EQ(total, 24);
    
    employee_manager_free(mgr);
}

// 测试employee_manager_yearly_attendance NULL
TEST(EmployeeManagerTest, YearlyAttendanceNull) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(employee_manager_yearly_attendance(nullptr, "2024"), 0);
    EXPECT_EQ(employee_manager_yearly_attendance(mgr, nullptr), 0);
    
    employee_manager_free(mgr);
}
