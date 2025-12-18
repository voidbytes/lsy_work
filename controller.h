#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"
#include "model.h"
#include "view.h"
#include "storage.h"

/* 控制器结构 */
typedef struct {
    EmployeeManager *manager;  /* 数据模型 */
    AppView *view;             /* 视图 */
    char *data_file;           /* 数据文件路径 */
    char *auth_file;           /* 认证文件路径 */
    Bool is_running;           /* 运行状态 */
} Controller;

/* 创建控制器 */
Controller *controller_create(const char *data_file, const char *auth_file);

/* 释放控制器 */
void controller_free(Controller *ctrl);

/* 启动系统 */
ErrorCode controller_start(Controller *ctrl);

/* 用户登录 */
Bool controller_login(Controller *ctrl);

/* 主循环 */
void controller_run(Controller *ctrl);

/* 处理菜单选项 */
void controller_handle_menu(Controller *ctrl, int choice);

/* ========== 菜单处理函数 ========== */
void controller_add_employee(Controller *ctrl);
void controller_remove_employee(Controller *ctrl);
void controller_update_employee(Controller *ctrl);
void controller_search_employee(Controller *ctrl);
void controller_show_all_employees(Controller *ctrl);
void controller_sort_employees(Controller *ctrl);
void controller_statistics(Controller *ctrl);
void controller_export_csv(Controller *ctrl);
void controller_save_and_exit(Controller *ctrl);

#endif /* CONTROLLER_H */
