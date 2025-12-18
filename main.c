#include "controller.h"
#include <stdio.h>

int main(void) {
    /* 创建控制器 */
    Controller *ctrl = controller_create("employees.db", "admin.auth");
    if (ctrl == NULL) {
        printf("System initialization failed!\n");
        return 1;
    }
    
    /* 启动系统(登录 + 加载数据) */
    ErrorCode err = controller_start(ctrl);
    if (err != SUCCESS) {
        printf("System startup failed!\n");
        controller_free(ctrl);
        return 1;
    }
    
    /* 运行主循环 */
    controller_run(ctrl);
    
    /* 清理资源 */
    controller_free(ctrl);
    
    printf("\nThank you for using Employee Management System!\n");
    return 0;
}
