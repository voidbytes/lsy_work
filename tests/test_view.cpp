#include <gtest/gtest.h>
extern "C" {
    #include "../view.h"
    #include "../model.h"
}

// 测试view_create
TEST(ViewTest, CreateView) {
    AppView *view = view_create_console();
    ASSERT_NE(view, nullptr);
    ASSERT_NE(view->vptr, nullptr);
    ASSERT_NE(view->vptr->show_menu, nullptr);
    ASSERT_NE(view->vptr->show_table_header, nullptr);
    ASSERT_NE(view->vptr->render_row, nullptr);
    ASSERT_NE(view->vptr->show_message, nullptr);
    ASSERT_NE(view->vptr->get_input_int, nullptr);
    ASSERT_NE(view->vptr->get_input_string, nullptr);
    view_free(view);
}

// 测试view_free
TEST(ViewTest, FreeView) {
    AppView *view = view_create_console();
    view_free(view);  // 不应该崩溃
    view_free(nullptr);  // 不应该崩溃
}

// 测试view_show_menu
TEST(ViewTest, ShowMenu) {
    AppView *view = view_create_console();
    ASSERT_NE(view, nullptr);
    view->vptr->show_menu(view);  // 不应该崩溃
    view_free(view);
}

// 测试view_show_table_header
TEST(ViewTest, ShowTableHeader) {
    AppView *view = view_create_console();
    ASSERT_NE(view, nullptr);
    view->vptr->show_table_header();  // 不应该崩溃
    view_free(view);
}

// 测试view_render_row
TEST(ViewTest, RenderRow) {
    Employee *emp = employee_create(1001, "张三", "研发部", "2024-01-15", 22);
    ASSERT_NE(emp, nullptr);
    
    AppView *view = view_create_console();
    ASSERT_NE(view, nullptr);
    view->vptr->render_row(emp);  // 不应该崩溃
    view->vptr->render_row(nullptr);  // 不应该崩溃
    view_free(view);
    
    employee_free(emp);
}

// 测试view_show_message
TEST(ViewTest, ShowMessage) {
    AppView *view = view_create_console();
    ASSERT_NE(view, nullptr);
    view->vptr->show_message("测试消息", FALSE);  // 不应该崩溃
    view->vptr->show_message("错误消息", TRUE);  // 不应该崩溃
    view->vptr->show_message(nullptr, FALSE);  // 不应该崩溃
    view_free(view);
}
