#include "view.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 清除输入缓冲区 */
void view_clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 控制台视图的具体实现函数 */
static void console_show_menu(AppView *view) {
    printf("\n");
    printf("========================================\n");
    printf("  Employee Management System v1.0\n");
    printf("========================================\n");
    printf("  1. Add Employee\n");
    printf("  2. Remove Employee\n");
    printf("  3. Update Employee\n");
    printf("  4. Search Employee\n");
    printf("  5. Show All Employees\n");
    printf("  6. Sort Employees\n");
    printf("  7. Attendance Statistics\n");
    printf("  8. Export to CSV\n");
    printf("  9. Save and Exit\n");
    printf("  0. Exit (without saving)\n");
    printf("========================================\n");
}

static void console_render_row(const Employee *emp) {
    if (emp != NULL) {
        employee_print(emp);
    }
}

static void console_show_message(const char *msg, Bool is_error) {
    if (msg != NULL) {
        if (is_error) {
            printf("\n[ERROR] %s\n", msg);
        } else {
            printf("\n[INFO] %s\n", msg);
        }
    }
}

static int console_get_input_int(const char *prompt) {
    int value;
    if (prompt != NULL) {
        printf("%s", prompt);
    }
    
    while (scanf("%d", &value) != 1) {
        view_clear_input_buffer();
        printf("Invalid input, please enter an integer: ");
    }
    
    view_clear_input_buffer();
    return value;
}

static void console_get_input_string(const char *prompt, char *buffer, size_t size) {
    if (prompt != NULL) {
        printf("%s", prompt);
    }
    
    if (buffer != NULL && size > 0) {
        if (fgets(buffer, size, stdin) != NULL) {
            /* 移除末尾的换行符 */
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
        }
    }
}

static void console_show_table_header(void) {
    printf("\n");
    printf("%-8s %-20s %-20s %-12s %-10s\n",
           "ID", "Name", "Department", "Attend Date", "Attend Days");
    printf("------------------------------------------------------------------------\n");
}

/* 控制台视图接口实现的静态虚表 */
static const ViewInterface console_interface = {
    .show_menu = console_show_menu,
    .render_row = console_render_row,
    .show_message = console_show_message,
    .get_input_int = console_get_input_int,
    .get_input_string = console_get_input_string,
    .show_table_header = console_show_table_header
};

/* 构造函数 */
AppView *view_create_console(void) {
    AppView *view = (AppView *)malloc(sizeof(AppView));
    if (view) {
        view->vptr = &console_interface; // 核心：绑定接口
        view->user_data = NULL;
    }
    return view;
}

/* 释放视图对象 */
void view_free(AppView *view) {
    if (view != NULL) {
        free(view);
    }
}

/* 向后兼容的函数：显示主菜单 */
void view_show_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("  Employee Management System v1.0\n");
    printf("========================================\n");
    printf("  1. Add Employee\n");
    printf("  2. Remove Employee\n");
    printf("  3. Update Employee\n");
    printf("  4. Search Employee\n");
    printf("  5. Show All Employees\n");
    printf("  6. Sort Employees\n");
    printf("  7. Attendance Statistics\n");
    printf("  8. Export to CSV\n");
    printf("  9. Save and Exit\n");
    printf("  0. Exit (without saving)\n");
    printf("========================================\n");
}

/* 显示表格头 */
void view_show_table_header(void) {
    printf("\n");
    printf("%-8s %-20s %-20s %-12s %-10s\n",
           "ID", "Name", "Department", "Attend Date", "Attend Days");
    printf("------------------------------------------------------------------------\n");
}

/* 渲染一行职工信息 */
void view_render_row(const Employee *emp) {
    if (emp != NULL) {
        employee_print(emp);
    }
}

/* 显示消息 */
void view_show_message(const char *msg, Bool is_error) {
    if (msg != NULL) {
        if (is_error) {
            printf("\n[ERROR] %s\n", msg);
        } else {
            printf("\n[INFO] %s\n", msg);
        }
    }
}

/* 获取整数输入 */
int view_get_input_int(const char *prompt) {
    int value;
    if (prompt != NULL) {
        printf("%s", prompt);
    }
    
    while (scanf("%d", &value) != 1) {
        view_clear_input_buffer();
        printf("Invalid input, please enter an integer: ");
    }
    
    view_clear_input_buffer();
    return value;
}

/* 获取字符串输入 */
void view_get_input_string(const char *prompt, char *buffer, size_t size) {
    if (prompt != NULL) {
        printf("%s", prompt);
    }
    
    if (buffer != NULL && size > 0) {
        if (fgets(buffer, size, stdin) != NULL) {
            /* 移除末尾的换行符 */
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
        }
    }
}

/* 等待用户按回车继续 */
void view_pause(void) {
    printf("\nPress Enter to continue...");
    getchar();
}