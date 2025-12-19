#include "model.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ========== Employee 工具函数实现 ========== */

Employee *employee_create(int id, const char *name, const char *department,
                         const char *attend_date, int attend_days) {
    if (name == NULL || department == NULL || attend_date == NULL) {
        return NULL;
    }
    
    Employee *emp = (Employee *)malloc(sizeof(Employee));
    if (emp == NULL) {
        return NULL;
    }
    
    emp->id = id;
    strncpy(emp->name, name, MAX_NAME_LEN - 1);
    emp->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(emp->department, department, MAX_DEPT_LEN - 1);
    emp->department[MAX_DEPT_LEN - 1] = '\0';
    strncpy(emp->attend_date, attend_date, MAX_DATE_LEN - 1);
    emp->attend_date[MAX_DATE_LEN - 1] = '\0';
    emp->attend_days = attend_days;
    
    return emp;
}

void employee_free(Employee *emp) {
    if (emp != NULL) {
        free(emp);
    }
}

void employee_print(const Employee *emp) {
    if (emp != NULL) {
        printf("%-8d %-20s %-20s %-12s %-10d\n",
               emp->id, emp->name, emp->department,
               emp->attend_date, emp->attend_days);
    }
}

/* ========== EmployeeManager 实现 ========== */

EmployeeManager *employee_manager_create(void) {
    EmployeeManager *manager = (EmployeeManager *)malloc(sizeof(EmployeeManager));
    if (manager == NULL) {
        return NULL;
    }
    
    manager->employees = vector_create();
    if (manager->employees == NULL) {
        free(manager);
        return NULL;
    }
    
    manager->next_id = 1001;  /* 工号从1001开始 */
    return manager;
}

void employee_manager_free(EmployeeManager *manager) {
    if (manager != NULL) {
        if (manager->employees != NULL) {
            /* 释放所有职工对象 */
            size_t size = vector_size(manager->employees);
            for (size_t i = 0; i < size; i++) {
                Employee *emp = (Employee *)manager->employees->data[i];
                employee_free(emp);
            }
            vector_free(manager->employees);
        }
        free(manager);
    }
}

ErrorCode employee_manager_add(EmployeeManager *manager, const char *name,
                               const char *department, const char *attend_date,
                               int attend_days) {
    if (manager == NULL || name == NULL || department == NULL || attend_date == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (attend_days < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    Employee *emp = employee_create(manager->next_id, name, department,
                                    attend_date, attend_days);
    if (emp == NULL) {
        return ERROR_OUT_OF_MEMORY;
    }
    
    ErrorCode err = vector_push_back(manager->employees, emp);
    if (err != SUCCESS) {
        employee_free(emp);
        return err;
    }
    
    manager->next_id++;
    return SUCCESS;
}

ErrorCode employee_manager_remove_at(EmployeeManager *manager, size_t index) {
    if (manager == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (index >= vector_size(manager->employees)) {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }
    
    Employee *emp = (Employee *)manager->employees->data[index];
    employee_free(emp);
    
    return vector_remove_at(manager->employees, index);
}

ErrorCode employee_manager_remove_by_id(EmployeeManager *manager, int id) {
    if (manager == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    size_t size = vector_size(manager->employees);
    for (size_t i = 0; i < size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        if (emp->id == id) {
            return employee_manager_remove_at(manager, i);
        }
    }
    
    return ERROR_NOT_FOUND;
}

ErrorCode employee_manager_update(EmployeeManager *manager, int id,
                                  const char *name, const char *department,
                                  const char *attend_date, int attend_days) {
    if (manager == NULL || name == NULL || department == NULL || attend_date == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    if (attend_days < 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    size_t size = vector_size(manager->employees);
    for (size_t i = 0; i < size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        if (emp->id == id) {
            strncpy(emp->name, name, MAX_NAME_LEN - 1);
            emp->name[MAX_NAME_LEN - 1] = '\0';
            strncpy(emp->department, department, MAX_DEPT_LEN - 1);
            emp->department[MAX_DEPT_LEN - 1] = '\0';
            strncpy(emp->attend_date, attend_date, MAX_DATE_LEN - 1);
            emp->attend_date[MAX_DATE_LEN - 1] = '\0';
            emp->attend_days = attend_days;
            return SUCCESS;
        }
    }
    
    return ERROR_NOT_FOUND;
}

Vector *employee_manager_search(EmployeeManager *manager, SearchType type,
                                const void *keyword) {
    if (manager == NULL || keyword == NULL) {
        return NULL;
    }
    
    Vector *results = vector_create();
    if (results == NULL) {
        return NULL;
    }
    
    size_t size = vector_size(manager->employees);
    for (size_t i = 0; i < size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        Bool match = FALSE;
        
        switch (type) {
            case SEARCH_BY_ID:
                if (emp->id == *(int *)keyword) {
                    match = TRUE;
                }
                break;
            case SEARCH_BY_NAME:
                if (strstr(emp->name, (const char *)keyword) != NULL) {
                    match = TRUE;
                }
                break;
            case SEARCH_BY_DEPARTMENT:
                if (strcmp(emp->department, (const char *)keyword) == 0) {
                    match = TRUE;
                }
                break;
        }
        
        if (match) {
            vector_push_back(results, emp);
        }
    }
    
    return results;
}

/* 比较器函数 */
static int compare_by_id(const void *a, const void *b) {
    Employee *e1 = *(Employee **)a;
    Employee *e2 = *(Employee **)b;
    /* 防止整数溢出的安全比较 */
    return (e1->id > e2->id) - (e1->id < e2->id);
}

static int compare_by_name(const void *a, const void *b) {
    Employee *e1 = *(Employee **)a;
    Employee *e2 = *(Employee **)b;
    return strcmp(e1->name, e2->name);
}

static int compare_by_department(const void *a, const void *b) {
    Employee *e1 = *(Employee **)a;
    Employee *e2 = *(Employee **)b;
    return strcmp(e1->department, e2->department);
}

static int compare_by_attend_date(const void *a, const void *b) {
    Employee *e1 = *(Employee **)a;
    Employee *e2 = *(Employee **)b;
    return strcmp(e1->attend_date, e2->attend_date);
}

static int compare_by_attend_days(const void *a, const void *b) {
    Employee *e1 = *(Employee **)a;
    Employee *e2 = *(Employee **)b;
    /* 降序: e2 > e1 返回 1，防止整数溢出的安全比较 */
    return (e2->attend_days > e1->attend_days) - (e2->attend_days < e1->attend_days);
}

void employee_manager_sort(EmployeeManager *manager, SortType type) {
    if (manager == NULL || manager->employees == NULL) {
        return;
    }
    
    Comparator compare = NULL;
    switch (type) {
        case SORT_BY_ID:
            compare = compare_by_id;
            break;
        case SORT_BY_NAME:
            compare = compare_by_name;
            break;
        case SORT_BY_DEPARTMENT:
            compare = compare_by_department;
            break;
        case SORT_BY_ATTEND_DATE:
            compare = compare_by_attend_date;
            break;
        case SORT_BY_ATTEND_DAYS:
            compare = compare_by_attend_days;
            break;
    }
    
    if (compare != NULL) {
        quick_sort(manager->employees, compare);
    }
}

Vector *employee_manager_get_all(EmployeeManager *manager) {
    if (manager == NULL) {
        return NULL;
    }
    return manager->employees;
}

int employee_manager_monthly_attendance(EmployeeManager *manager,
                                        const char *year_month) {
    if (manager == NULL || year_month == NULL) {
        return 0;
    }
    
    int total_days = 0;
    size_t len = strlen(year_month);
    
    size_t size = vector_size(manager->employees);
    for (size_t i = 0; i < size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        /* 检查日期是否以year_month开头 (如"2024-01") */
        if (strncmp(emp->attend_date, year_month, len) == 0) {
            total_days += emp->attend_days;
        }
    }
    
    return total_days;
}

int employee_manager_yearly_attendance(EmployeeManager *manager,
                                       const char *year) {
    if (manager == NULL || year == NULL) {
        return 0;
    }
    
    int total_days = 0;
    size_t len = strlen(year);
    
    size_t size = vector_size(manager->employees);
    for (size_t i = 0; i < size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        /* 检查日期是否以year开头 (如"2024") */
        if (strncmp(emp->attend_date, year, len) == 0) {
            total_days += emp->attend_days;
        }
    }
    
    return total_days;
}
