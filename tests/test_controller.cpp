#include <gtest/gtest.h>
#include <cstdio>
extern "C" {
    #include "../controller.h"
}

const char *TEST_CTRL_DB = "test_ctrl.db";
const char *TEST_CTRL_AUTH = "test_ctrl.auth";

// 测试夹具
class ControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove(TEST_CTRL_DB);
        std::remove(TEST_CTRL_AUTH);
    }
    
    void TearDown() override {
        std::remove(TEST_CTRL_DB);
        std::remove(TEST_CTRL_AUTH);
    }
};

// 测试controller_create
TEST_F(ControllerTest, CreateController) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    ASSERT_NE(ctrl->manager, nullptr);
    ASSERT_NE(ctrl->view, nullptr);
    ASSERT_NE(ctrl->data_file, nullptr);
    ASSERT_NE(ctrl->auth_file, nullptr);
    EXPECT_EQ(ctrl->is_running, TRUE);
    controller_free(ctrl);
}

// 测试controller_create NULL参数
TEST_F(ControllerTest, CreateControllerNullParams) {
    EXPECT_EQ(controller_create(nullptr, TEST_CTRL_AUTH), nullptr);
    EXPECT_EQ(controller_create(TEST_CTRL_DB, nullptr), nullptr);
    EXPECT_EQ(controller_create(nullptr, nullptr), nullptr);
}

// 测试controller_free
TEST_F(ControllerTest, FreeController) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    controller_free(ctrl);  // 不应该崩溃
    controller_free(nullptr);  // 不应该崩溃
}

// 测试controller_handle_menu
TEST_F(ControllerTest, HandleMenuInvalidChoice) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    
    // 测试无效选项
    controller_handle_menu(ctrl, 100);  // 不应该崩溃
    controller_handle_menu(ctrl, -1);   // 不应该崩溃
    
    controller_free(ctrl);
}

// 测试controller_handle_menu 退出选项
TEST_F(ControllerTest, HandleMenuExit) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    
    ctrl->is_running = TRUE;
    controller_handle_menu(ctrl, 0);  // 退出不保存
    EXPECT_EQ(ctrl->is_running, FALSE);
    
    controller_free(ctrl);
}

// 测试controller_add_employee (需要模拟输入,这里只测试函数不崩溃)
TEST_F(ControllerTest, AddEmployeeFunction) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    
    // 直接测试manager的add功能,而不是通过controller的交互式函数
    ErrorCode err = employee_manager_add(ctrl->manager, "测试员工", "测试部门", "2024-01-15", 20);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(ctrl->manager->employees->size, 1);
    
    controller_free(ctrl);
}

// 测试controller_save_and_exit
TEST_F(ControllerTest, SaveAndExit) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    
    // 添加一些数据
    employee_manager_add(ctrl->manager, "张三", "研发部", "2024-01-15", 22);
    
    ctrl->is_running = TRUE;
    controller_save_and_exit(ctrl);
    
    // 验证文件被创建
    FILE *fp = fopen(TEST_CTRL_DB, "rb");
    ASSERT_NE(fp, nullptr);
    fclose(fp);
    
    controller_free(ctrl);
}

// 测试NULL controller的各种操作
TEST_F(ControllerTest, NullControllerOperations) {
    EXPECT_EQ(controller_start(nullptr), ERROR_NULL_POINTER);
    
    controller_run(nullptr);  // 不应该崩溃
    controller_handle_menu(nullptr, 1);  // 不应该崩溃
    controller_save_and_exit(nullptr);  // 不应该崩溃
}

// 测试controller基本操作流程
TEST_F(ControllerTest, BasicOperationFlow) {
    Controller *ctrl = controller_create(TEST_CTRL_DB, TEST_CTRL_AUTH);
    ASSERT_NE(ctrl, nullptr);
    
    // 添加员工
    employee_manager_add(ctrl->manager, "张三", "研发部", "2024-01-15", 22);
    employee_manager_add(ctrl->manager, "李四", "市场部", "2024-01-16", 23);
    EXPECT_EQ(ctrl->manager->employees->size, 2);
    
    // 删除员工
    ErrorCode err = employee_manager_remove_by_id(ctrl->manager, 1001);
    EXPECT_EQ(err, SUCCESS);
    EXPECT_EQ(ctrl->manager->employees->size, 1);
    
    // 更新员工
    err = employee_manager_update(ctrl->manager, 1002, "李四四", "销售部", "2024-02-01", 25);
    EXPECT_EQ(err, SUCCESS);
    
    Employee *emp = (Employee *)ctrl->manager->employees->data[0];
    EXPECT_STREQ(emp->name, "李四四");
    
    controller_free(ctrl);
}


