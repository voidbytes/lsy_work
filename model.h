#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "vector.h"
#include "sort.h"

/* 职工结构体 */
PACK_PUSH
typedef struct {
    int id;                          /* 工号 */
    char name[MAX_NAME_LEN];         /* 姓名 */
    char department[MAX_DEPT_LEN];   /* 部门 */
    char attend_date[MAX_DATE_LEN];  /* 出勤日期 (YYYY-MM-DD) */
    int attend_days;                 /* 出勤天数 */
} Employee;
PACK_POP

/* 职工管理器 */
typedef struct {
    Vector *employees;  /* 存储Employee指针的动态数组 */
    int next_id;        /* 下一个可用的工号 */
} EmployeeManager;

/* 查询条件 */
typedef enum {
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_DEPARTMENT
} SearchType;

/* 排序条件 */
typedef enum {
    SORT_BY_ID,
    SORT_BY_NAME,
    SORT_BY_DEPARTMENT,
    SORT_BY_ATTEND_DATE,
    SORT_BY_ATTEND_DAYS
} SortType;

/* ========== EmployeeManager 方法 ========== */

/* 创建管理器 */
EmployeeManager *employee_manager_create(void);

/* 释放管理器 */
void employee_manager_free(EmployeeManager *manager);

/* 添加职工 */
ErrorCode employee_manager_add(EmployeeManager *manager, const char *name, 
                               const char *department, const char *attend_date, 
                               int attend_days);

/* 根据索引删除职工 */
ErrorCode employee_manager_remove_at(EmployeeManager *manager, size_t index);

/* 根据工号删除职工 */
ErrorCode employee_manager_remove_by_id(EmployeeManager *manager, int id);

/* 修改职工信息 */
ErrorCode employee_manager_update(EmployeeManager *manager, int id,
                                  const char *name, const char *department,
                                  const char *attend_date, int attend_days);

/* 查询职工 */
Vector *employee_manager_search(EmployeeManager *manager, SearchType type, 
                                const void *keyword);

/* 排序职工 */
void employee_manager_sort(EmployeeManager *manager, SortType type);

/* 获取所有职工 */
Vector *employee_manager_get_all(EmployeeManager *manager);

/* 统计月度出勤 */
int employee_manager_monthly_attendance(EmployeeManager *manager, 
                                        const char *year_month);

/* 统计年度出勤 */
int employee_manager_yearly_attendance(EmployeeManager *manager, 
                                       const char *year);

/* ========== Employee 工具函数 ========== */

/* 创建职工 */
Employee *employee_create(int id, const char *name, const char *department,
                         const char *attend_date, int attend_days);

/* 释放职工 */
void employee_free(Employee *emp);

/* 打印职工信息 */
void employee_print(const Employee *emp);

#endif /* MODEL_H */
