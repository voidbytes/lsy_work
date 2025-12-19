#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
extern "C" {
    #include "../storage.h"
    #include "../model.h"
}

// 测试文件路径
const char *TEST_DB_FILE = "test_employees.db";
const char *TEST_AUTH_FILE = "test_admin.auth";
const char *TEST_CSV_FILE = "test_export.csv";

// 测试夹具
class StorageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 清理测试文件
        std::remove(TEST_DB_FILE);
        std::remove(TEST_AUTH_FILE);
        std::remove(TEST_CSV_FILE);
    }
    
    void TearDown() override {
        // 清理测试文件
        std::remove(TEST_DB_FILE);
        std::remove(TEST_AUTH_FILE);
        std::remove(TEST_CSV_FILE);
    }
};

// 测试storage_save_employees和storage_load_employees
TEST_F(StorageTest, SaveAndLoadEmployees) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    // 添加一些员工
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    employee_manager_add(mgr, "王五", "财务部", "2024-01-17", 24);
    
    // 保存到文件
    ErrorCode err = storage_save_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 创建新的manager并加载数据
    EmployeeManager *mgr2 = employee_manager_create();
    ASSERT_NE(mgr2, nullptr);
    
    err = storage_load_employees(TEST_DB_FILE, mgr2);
    EXPECT_EQ(err, SUCCESS);
    
    // 验证数据
    EXPECT_EQ(mgr2->employees->size, 3);
    EXPECT_EQ(mgr2->next_id, mgr->next_id);
    
    Employee *emp = (Employee *)mgr2->employees->data[0];
    EXPECT_EQ(emp->id, 1001);
    EXPECT_STREQ(emp->name, "张三");
    
    employee_manager_free(mgr);
    employee_manager_free(mgr2);
}

// 测试storage_save_employees NULL参数
TEST_F(StorageTest, SaveEmployeesNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(storage_save_employees(nullptr, mgr), ERROR_NULL_POINTER);
    EXPECT_EQ(storage_save_employees(TEST_DB_FILE, nullptr), ERROR_NULL_POINTER);
    
    employee_manager_free(mgr);
}

// 测试storage_load_employees NULL参数
TEST_F(StorageTest, LoadEmployeesNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(storage_load_employees(nullptr, mgr), ERROR_NULL_POINTER);
    EXPECT_EQ(storage_load_employees(TEST_DB_FILE, nullptr), ERROR_NULL_POINTER);
    
    employee_manager_free(mgr);
}

// 测试storage_load_employees 文件不存在
TEST_F(StorageTest, LoadEmployeesFileNotFound) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = storage_load_employees("nonexistent_file.db", mgr);
    EXPECT_EQ(err, ERROR_FILE_NOT_FOUND);
    
    employee_manager_free(mgr);
}

// 测试storage_load_employees 无效文件(魔数错误)
TEST_F(StorageTest, LoadEmployeesInvalidMagic) {
    // 创建一个包含错误魔数的文件
    FILE *fp = fopen(TEST_DB_FILE, "wb");
    ASSERT_NE(fp, nullptr);
    
    FileHeader header;
    header.magic = 0xDEADBEEF;  // 错误的魔数
    header.version = FILE_VERSION;
    header.count = 0;
    header.checksum = 0;
    
    fwrite(&header, sizeof(FileHeader), 1, fp);
    fclose(fp);
    
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = storage_load_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, ERROR_INVALID_FILE);
    
    employee_manager_free(mgr);
}

// 测试storage_load_employees 无效文件(版本错误)
TEST_F(StorageTest, LoadEmployeesInvalidVersion) {
    // 创建一个包含错误版本的文件
    FILE *fp = fopen(TEST_DB_FILE, "wb");
    ASSERT_NE(fp, nullptr);
    
    FileHeader header;
    header.magic = MAGIC_NUMBER;
    header.version = 999;  // 错误的版本
    header.count = 0;
    header.checksum = 0;
    
    fwrite(&header, sizeof(FileHeader), 1, fp);
    fclose(fp);
    
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = storage_load_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, ERROR_INVALID_FILE);
    
    employee_manager_free(mgr);
}

// 测试storage_export_csv
TEST_F(StorageTest, ExportCsv) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    ErrorCode err = storage_export_csv(TEST_CSV_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 验证文件存在
    FILE *fp = fopen(TEST_CSV_FILE, "r");
    ASSERT_NE(fp, nullptr);
    
    char line[256];
    // 读取标题行
    ASSERT_NE(fgets(line, sizeof(line), fp), nullptr);
    
    // 读取第一行数据
    ASSERT_NE(fgets(line, sizeof(line), fp), nullptr);
    EXPECT_TRUE(strstr(line, "张三") != nullptr);
    
    fclose(fp);
    employee_manager_free(mgr);
}

// 测试storage_export_csv NULL参数
TEST_F(StorageTest, ExportCsvNullParams) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    EXPECT_EQ(storage_export_csv(nullptr, mgr), ERROR_NULL_POINTER);
    EXPECT_EQ(storage_export_csv(TEST_CSV_FILE, nullptr), ERROR_NULL_POINTER);
    
    employee_manager_free(mgr);
}

// 测试storage_save_credential和storage_verify_credential
TEST_F(StorageTest, SaveAndVerifyCredential) {
    const char *username = "admin";
    const char *password = "password123";
    
    ErrorCode err = storage_save_credential(TEST_AUTH_FILE, username, password);
    EXPECT_EQ(err, SUCCESS);
    
    // 验证正确的凭证
    Bool result = storage_verify_credential(TEST_AUTH_FILE, username, password);
    EXPECT_EQ(result, TRUE);
    
    // 验证错误的密码
    result = storage_verify_credential(TEST_AUTH_FILE, username, "wrongpassword");
    EXPECT_EQ(result, FALSE);
    
    // 验证错误的用户名
    result = storage_verify_credential(TEST_AUTH_FILE, "wronguser", password);
    EXPECT_EQ(result, FALSE);
}

// 测试storage_save_credential NULL参数
TEST_F(StorageTest, SaveCredentialNullParams) {
    EXPECT_EQ(storage_save_credential(nullptr, "admin", "password"), ERROR_NULL_POINTER);
    EXPECT_EQ(storage_save_credential(TEST_AUTH_FILE, nullptr, "password"), ERROR_NULL_POINTER);
    EXPECT_EQ(storage_save_credential(TEST_AUTH_FILE, "admin", nullptr), ERROR_NULL_POINTER);
}

// 测试storage_verify_credential NULL参数
TEST_F(StorageTest, VerifyCredentialNullParams) {
    EXPECT_EQ(storage_verify_credential(nullptr, "admin", "password"), FALSE);
    EXPECT_EQ(storage_verify_credential(TEST_AUTH_FILE, nullptr, "password"), FALSE);
    EXPECT_EQ(storage_verify_credential(TEST_AUTH_FILE, "admin", nullptr), FALSE);
}

// 测试storage_verify_credential 文件不存在
TEST_F(StorageTest, VerifyCredentialFileNotFound) {
    Bool result = storage_verify_credential("nonexistent.auth", "admin", "password");
    EXPECT_EQ(result, FALSE);
}

// 测试空manager保存和加载
TEST_F(StorageTest, SaveAndLoadEmptyManager) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    // 保存空manager
    ErrorCode err = storage_save_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 加载空manager
    EmployeeManager *mgr2 = employee_manager_create();
    ASSERT_NE(mgr2, nullptr);
    
    err = storage_load_employees(TEST_DB_FILE, mgr2);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr2->employees->size, 0);
    
    employee_manager_free(mgr);
    employee_manager_free(mgr2);
}

// 测试大量数据保存和加载
TEST_F(StorageTest, SaveAndLoadLargeData) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    // 添加100个员工
    for (int i = 0; i < 100; i++) {
        char name[32], dept[32], date[32];
        snprintf(name, sizeof(name), "员工%d", i);
        snprintf(dept, sizeof(dept), "部门%d", i % 10);
        snprintf(date, sizeof(date), "2024-01-%02d", (i % 28) + 1);
        
        employee_manager_add(mgr, name, dept, date, 20 + (i % 10));
    }
    
    EXPECT_EQ(mgr->employees->size, 100);
    
    // 保存
    ErrorCode err = storage_save_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 加载
    EmployeeManager *mgr2 = employee_manager_create();
    ASSERT_NE(mgr2, nullptr);
    
    err = storage_load_employees(TEST_DB_FILE, mgr2);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr2->employees->size, 100);
    
    // 验证一些数据
    Employee *emp = (Employee *)mgr2->employees->data[0];
    EXPECT_STREQ(emp->name, "员工0");
    
    employee_manager_free(mgr);
    employee_manager_free(mgr2);
}

// 测试CSV导出空manager
TEST_F(StorageTest, ExportCsvEmptyManager) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    ErrorCode err = storage_export_csv(TEST_CSV_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 验证文件存在并且只有标题行
    FILE *fp = fopen(TEST_CSV_FILE, "r");
    ASSERT_NE(fp, nullptr);
    
    char line[256];
    ASSERT_NE(fgets(line, sizeof(line), fp), nullptr);  // 标题行
    EXPECT_EQ(fgets(line, sizeof(line), fp), nullptr);  // 应该没有数据行
    
    fclose(fp);
    employee_manager_free(mgr);
}

// 测试加载时没有next_id的情况
TEST_F(StorageTest, LoadEmployeesWithoutNextId) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(mgr, "李四", "市场部", "2024-01-16", 23);
    
    // 正常保存和加载,确保兼容性
    ErrorCode err = storage_save_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    EmployeeManager *mgr2 = employee_manager_create();
    ASSERT_NE(mgr2, nullptr);
    
    err = storage_load_employees(TEST_DB_FILE, mgr2);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(mgr2->employees->size, 2);
    EXPECT_EQ(mgr2->next_id, 1003);
    
    employee_manager_free(mgr);
    employee_manager_free(mgr2);
}

// 测试校验和错误
TEST_F(StorageTest, LoadEmployeesChecksumError) {
    EmployeeManager *mgr = employee_manager_create();
    ASSERT_NE(mgr, nullptr);
    
    employee_manager_add(mgr, "张三", "研发部", "2024-01-15", 22);
    
    // 保存
    ErrorCode err = storage_save_employees(TEST_DB_FILE, mgr);
    EXPECT_EQ(err, SUCCESS);
    
    // 破坏文件，修改数据部分
    FILE *fp = fopen(TEST_DB_FILE, "r+b");
    ASSERT_NE(fp, nullptr);
    
    FileHeader header;
    fread(&header, sizeof(FileHeader), 1, fp);
    
    Employee emp;
    fread(&emp, sizeof(Employee), 1, fp);
    
    // 修改员工数据
    strcpy(emp.name, "Modified");
    
    // 写回修改后的数据
    fseek(fp, sizeof(FileHeader), SEEK_SET);
    fwrite(&emp, sizeof(Employee), 1, fp);
    fclose(fp);
    
    // 尝试加载,应该失败(校验和不匹配)
    EmployeeManager *mgr2 = employee_manager_create();
    ASSERT_NE(mgr2, nullptr);
    
    err = storage_load_employees(TEST_DB_FILE, mgr2);
    EXPECT_EQ(err, ERROR_DATA_CORRUPTION);
    
    employee_manager_free(mgr);
    employee_manager_free(mgr2);
}
