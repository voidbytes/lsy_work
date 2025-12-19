#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "model.h"

/* 前置声明：AppView结构体 */
struct AppView;

/* 视图接口结构体：定义视图组件的所有行为 */
typedef struct {
    void (*show_menu)(struct AppView *view);           /* 显示主菜单 */
    void (*render_row)(const Employee *emp);           /* 渲染一行职工信息 */
    void (*show_message)(const char *msg, Bool is_error); /* 显示消息 */
    int  (*get_input_int)(const char *prompt);         /* 获取整数输入 */
    void (*get_input_string)(const char *prompt, char *buffer, size_t size); /* 获取字符串输入 */
    void (*show_table_header)(void);                   /* 显示表格头部 */
} ViewInterface;

/* 视图对象结构体：包含接口指针和用户数据 */
typedef struct AppView {
    const ViewInterface *vptr;  /* 指向视图接口的指针 */
    void *user_data;            /* 预留：存储特定UI状态 */
} AppView;

/* 视图创建与释放函数 */
AppView *view_create_console(void); /* 创建控制台视图 */
void view_free(AppView *view);       /* 释放视图资源 */

/* 其他函数声明 */
void view_pause(void); /* 暂停并等待用户输入 */

/* 便捷宏：简化视图函数调用 */
#define VIEW_SHOW_MENU(view) (view)->vptr->show_menu(view)
#define VIEW_GET_INT(view, prompt) (view)->vptr->get_input_int(prompt)

#endif /* VIEW_H */