#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "model.h"

/* 视图类结构 */
typedef struct {
    /* 显示主菜单 */
    void (*show_menu)(void);
    
    /* 显示表格头 */
    void (*show_table_header)(void);
    
    /* 渲染一行职工信息 */
    void (*render_row)(const Employee *emp);
    
    /* 显示消息 */
    void (*show_message)(const char *msg, Bool is_error);
    
    /* 获取整数输入 */
    int (*get_input_int)(const char *prompt);
    
    /* 获取字符串输入 */
    void (*get_input_string)(const char *prompt, char *buffer, size_t size);
} AppView;

/* 创建视图对象 */
AppView *view_create(void);

/* 释放视图对象 */
void view_free(AppView *view);

/* 视图方法实现 */
void view_show_menu(void);
void view_show_table_header(void);
void view_render_row(const Employee *emp);
void view_show_message(const char *msg, Bool is_error);
int view_get_input_int(const char *prompt);
void view_get_input_string(const char *prompt, char *buffer, size_t size);

/* 清除输入缓冲区 */
void view_clear_input_buffer(void);

/* 等待用户按回车继续 */
void view_pause(void);

#endif /* VIEW_H */
