#include "controller.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 创建控制器 */
Controller *controller_create(const char *data_file, const char *auth_file) {
    if (data_file == NULL || auth_file == NULL) {
        return NULL;
    }
    
    Controller *ctrl = (Controller *)malloc(sizeof(Controller));
    if (ctrl == NULL) {
        return NULL;
    }
    
    ctrl->manager = employee_manager_create();
    if (ctrl->manager == NULL) {
        free(ctrl);
        return NULL;
    }
    
    ctrl->view = view_create();
    if (ctrl->view == NULL) {
        employee_manager_free(ctrl->manager);
        free(ctrl);
        return NULL;
    }
    
    ctrl->data_file = (char *)malloc(strlen(data_file) + 1);
    ctrl->auth_file = (char *)malloc(strlen(auth_file) + 1);
    
    if (ctrl->data_file == NULL || ctrl->auth_file == NULL) {
        view_free(ctrl->view);
        employee_manager_free(ctrl->manager);
        free(ctrl);
        return NULL;
    }
    
    strcpy(ctrl->data_file, data_file);
    strcpy(ctrl->auth_file, auth_file);
    ctrl->is_running = TRUE;
    
    return ctrl;
}

/* 释放控制器 */
void controller_free(Controller *ctrl) {
    if (ctrl != NULL) {
        if (ctrl->manager != NULL) {
            employee_manager_free(ctrl->manager);
        }
        if (ctrl->view != NULL) {
            view_free(ctrl->view);
        }
        if (ctrl->data_file != NULL) {
            free(ctrl->data_file);
        }
        if (ctrl->auth_file != NULL) {
            free(ctrl->auth_file);
        }
        free(ctrl);
    }
}

/* 用户登录 */
Bool controller_login(Controller *ctrl) {
    if (ctrl == NULL) {
        return FALSE;
    }
    
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    
    printf("\n========================================\n");
    printf("         System Login\n");
    printf("========================================\n");
    
    /* 检查是否已有管理员账号 */
    if (storage_verify_credential(ctrl->auth_file, "", "") == FALSE) {
        /* 首次运行,创建管理员账号 */
        printf("\nFirst run detected, please create admin account:\n");
        ctrl->view->get_input_string("Enter username: ", username, MAX_USERNAME_LEN);
        ctrl->view->get_input_string("Enter password: ", password, MAX_PASSWORD_LEN);
        
        if (storage_save_credential(ctrl->auth_file, username, password) != SUCCESS) {
            ctrl->view->show_message("Failed to create admin account!", TRUE);
            return FALSE;
        }
        
        ctrl->view->show_message("Admin account created successfully!", FALSE);
        return TRUE;
    }
    
    /* 登录验证 */
    int attempts = 3;
    while (attempts > 0) {
        ctrl->view->get_input_string("Username: ", username, MAX_USERNAME_LEN);
        ctrl->view->get_input_string("Password: ", password, MAX_PASSWORD_LEN);
        
        if (storage_verify_credential(ctrl->auth_file, username, password)) {
            ctrl->view->show_message("Login successful!", FALSE);
            return TRUE;
        }
        
        attempts--;
        if (attempts > 0) {
            printf("\n[ERROR] Invalid username or password! %d attempts remaining\n", attempts);
        }
    }
    
    ctrl->view->show_message("Too many failed login attempts, exiting!", TRUE);
    return FALSE;
}

/* 启动系统 */
ErrorCode controller_start(Controller *ctrl) {
    if (ctrl == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    /* 用户登录 */
    if (!controller_login(ctrl)) {
        return ERROR_AUTH_FAILED;
    }
    
    /* 尝试加载数据 */
    ErrorCode err = storage_load_employees(ctrl->data_file, ctrl->manager);
    if (err == ERROR_FILE_NOT_FOUND) {
        ctrl->view->show_message("Data file not found, will create new file", FALSE);
    } else if (err != SUCCESS) {
        ctrl->view->show_message("Failed to load data!", TRUE);
        return err;
    } else {
        char msg[100];
        snprintf(msg, 100, "Successfully loaded %zu employee records", 
                 ctrl->manager->employees->size);
        ctrl->view->show_message(msg, FALSE);
    }
    
    view_pause();
    return SUCCESS;
}

/* 主循环 */
void controller_run(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    while (ctrl->is_running) {
        ctrl->view->show_menu();
        int choice = ctrl->view->get_input_int("Select option (0-9): ");
        controller_handle_menu(ctrl, choice);
    }
}

/* 处理菜单选项 */
void controller_handle_menu(Controller *ctrl, int choice) {
    if (ctrl == NULL) {
        return;
    }
    
    switch (choice) {
        case 1:
            controller_add_employee(ctrl);
            break;
        case 2:
            controller_remove_employee(ctrl);
            break;
        case 3:
            controller_update_employee(ctrl);
            break;
        case 4:
            controller_search_employee(ctrl);
            break;
        case 5:
            controller_show_all_employees(ctrl);
            break;
        case 6:
            controller_sort_employees(ctrl);
            break;
        case 7:
            controller_statistics(ctrl);
            break;
        case 8:
            controller_export_csv(ctrl);
            break;
        case 9:
            controller_save_and_exit(ctrl);
            break;
        case 0:
            ctrl->view->show_message("Exit without saving", FALSE);
            ctrl->is_running = FALSE;
            break;
        default:
            ctrl->view->show_message("Invalid option!", TRUE);
            break;
    }
}

/* 添加职工 */
void controller_add_employee(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    char name[MAX_NAME_LEN];
    char department[MAX_DEPT_LEN];
    char attend_date[MAX_DATE_LEN];
    
    printf("\n========== Add Employee ==========\n");
    ctrl->view->get_input_string("Name: ", name, MAX_NAME_LEN);
    ctrl->view->get_input_string("Department: ", department, MAX_DEPT_LEN);
    ctrl->view->get_input_string("Attendance Date (YYYY-MM-DD): ", attend_date, MAX_DATE_LEN);
    int attend_days = ctrl->view->get_input_int("Attendance Days: ");
    
    ErrorCode err = employee_manager_add(ctrl->manager, name, department, 
                                         attend_date, attend_days);
    if (err == SUCCESS) {
        ctrl->view->show_message("Employee added successfully!", FALSE);
    } else {
        ctrl->view->show_message("Failed to add employee!", TRUE);
    }
    
    view_pause();
}

/* 删除职工 */
void controller_remove_employee(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    printf("\n========== Remove Employee ==========\n");
    int id = ctrl->view->get_input_int("Enter employee ID to remove: ");
    
    ErrorCode err = employee_manager_remove_by_id(ctrl->manager, id);
    if (err == SUCCESS) {
        ctrl->view->show_message("Employee removed successfully!", FALSE);
    } else if (err == ERROR_NOT_FOUND) {
        ctrl->view->show_message("Employee ID not found!", TRUE);
    } else {
        ctrl->view->show_message("Failed to remove employee!", TRUE);
    }
    
    view_pause();
}

/* 修改职工信息 */
void controller_update_employee(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    char name[MAX_NAME_LEN];
    char department[MAX_DEPT_LEN];
    char attend_date[MAX_DATE_LEN];
    
    printf("\n========== Update Employee ==========\n");
    int id = ctrl->view->get_input_int("Enter employee ID to update: ");
    
    ctrl->view->get_input_string("New Name: ", name, MAX_NAME_LEN);
    ctrl->view->get_input_string("New Department: ", department, MAX_DEPT_LEN);
    ctrl->view->get_input_string("New Attendance Date (YYYY-MM-DD): ", attend_date, MAX_DATE_LEN);
    int attend_days = ctrl->view->get_input_int("New Attendance Days: ");
    
    ErrorCode err = employee_manager_update(ctrl->manager, id, name, 
                                            department, attend_date, attend_days);
    if (err == SUCCESS) {
        ctrl->view->show_message("Employee updated successfully!", FALSE);
    } else if (err == ERROR_NOT_FOUND) {
        ctrl->view->show_message("Employee ID not found!", TRUE);
    } else {
        ctrl->view->show_message("Failed to update employee!", TRUE);
    }
    
    view_pause();
}

/* 查询职工 */
void controller_search_employee(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    printf("\n========== Search Employee ==========\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Department\n");
    
    int choice = ctrl->view->get_input_int("Select search method: ");
    
    Vector *results = NULL;
    
    switch (choice) {
        case 1: {
            int id = ctrl->view->get_input_int("Enter ID: ");
            results = employee_manager_search(ctrl->manager, SEARCH_BY_ID, &id);
            break;
        }
        case 2: {
            char name[MAX_NAME_LEN];
            ctrl->view->get_input_string("Enter name (fuzzy search supported): ", name, MAX_NAME_LEN);
            results = employee_manager_search(ctrl->manager, SEARCH_BY_NAME, name);
            break;
        }
        case 3: {
            char dept[MAX_DEPT_LEN];
            ctrl->view->get_input_string("Enter department: ", dept, MAX_DEPT_LEN);
            results = employee_manager_search(ctrl->manager, SEARCH_BY_DEPARTMENT, dept);
            break;
        }
        default:
            ctrl->view->show_message("Invalid option!", TRUE);
            view_pause();
            return;
    }
    
    if (results != NULL && results->size > 0) {
        printf("\nSearch results (%zu found):\n", results->size);
        ctrl->view->show_table_header();
        for (size_t i = 0; i < results->size; i++) {
            Employee *emp = (Employee *)results->data[i];
            ctrl->view->render_row(emp);
        }
        vector_free(results);
    } else {
        ctrl->view->show_message("No matching employees found!", FALSE);
        if (results != NULL) {
            vector_free(results);
        }
    }
    
    view_pause();
}

/* 显示所有职工 */
void controller_show_all_employees(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    Vector *employees = employee_manager_get_all(ctrl->manager);
    
    if (employees == NULL || employees->size == 0) {
        ctrl->view->show_message("No employee records!", FALSE);
    } else {
        printf("\nAll Employees (%zu total):\n", employees->size);
        ctrl->view->show_table_header();
        for (size_t i = 0; i < employees->size; i++) {
            Employee *emp = (Employee *)employees->data[i];
            ctrl->view->render_row(emp);
        }
    }
    
    view_pause();
}

/* 排序职工 */
void controller_sort_employees(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    printf("\n========== Sort Employees ==========\n");
    printf("1. Sort by ID\n");
    printf("2. Sort by Name\n");
    printf("3. Sort by Department\n");
    printf("4. Sort by Attendance Date\n");
    printf("5. Sort by Attendance Days (Descending)\n");
    
    int choice = ctrl->view->get_input_int("Select sort method: ");
    
    SortType type;
    switch (choice) {
        case 1:
            type = SORT_BY_ID;
            break;
        case 2:
            type = SORT_BY_NAME;
            break;
        case 3:
            type = SORT_BY_DEPARTMENT;
            break;
        case 4:
            type = SORT_BY_ATTEND_DATE;
            break;
        case 5:
            type = SORT_BY_ATTEND_DAYS;
            break;
        default:
            ctrl->view->show_message("Invalid option!", TRUE);
            view_pause();
            return;
    }
    
    employee_manager_sort(ctrl->manager, type);
    ctrl->view->show_message("Sort completed!", FALSE);
    
    /* 显示排序结果 */
    controller_show_all_employees(ctrl);
}

/* 统计考勤 */
void controller_statistics(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    printf("\n========== Attendance Statistics ==========\n");
    printf("1. Monthly Statistics\n");
    printf("2. Yearly Statistics\n");
    
    int choice = ctrl->view->get_input_int("Select statistics type: ");
    
    char msg[200];
    
    switch (choice) {
        case 1: {
            char year_month[8];
            ctrl->view->get_input_string("Enter year-month (YYYY-MM): ", year_month, 8);
            int total = employee_manager_monthly_attendance(ctrl->manager, year_month);
            snprintf(msg, 200, "%s total attendance days: %d", year_month, total);
            ctrl->view->show_message(msg, FALSE);
            break;
        }
        case 2: {
            char year[5];
            ctrl->view->get_input_string("Enter year (YYYY): ", year, 5);
            int total = employee_manager_yearly_attendance(ctrl->manager, year);
            snprintf(msg, 200, "%s total attendance days: %d", year, total);
            ctrl->view->show_message(msg, FALSE);
            break;
        }
        default:
            ctrl->view->show_message("Invalid option!", TRUE);
            break;
    }
    
    view_pause();
}

/* 导出CSV */
void controller_export_csv(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    char filename[256];
    
    printf("\n========== Export to CSV ==========\n");
    ctrl->view->get_input_string("Enter export filename: ", filename, 256);
    
    ErrorCode err = storage_export_csv(filename, ctrl->manager);
    if (err == SUCCESS) {
        ctrl->view->show_message("Export successful!", FALSE);
    } else {
        ctrl->view->show_message("Export failed!", TRUE);
    }
    
    view_pause();
}

/* 保存并退出 */
void controller_save_and_exit(Controller *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    
    ErrorCode err = storage_save_employees(ctrl->data_file, ctrl->manager);
    if (err == SUCCESS) {
        ctrl->view->show_message("Data saved successfully!", FALSE);
        ctrl->is_running = FALSE;
    } else {
        ctrl->view->show_message("Failed to save data!", TRUE);
    }
}
