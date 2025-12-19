#ifndef STORAGE_H
#define STORAGE_H

#include "common.h"
#include "model.h"

/* 文件头结构 */
PACK_PUSH
typedef struct {
    unsigned int magic;      /* 魔数: 0x454D5053 (ASCII: EMPS) */
    unsigned int version;    /* 版本号 */
    unsigned int count;      /* 记录条数 */
    unsigned int checksum;   /* 校验和 */
} FileHeader;
PACK_POP

/* 用户凭证结构 */
PACK_PUSH
typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
} UserCredential;
PACK_POP

/* ========== 数据存储函数 ========== */

/* 保存职工数据到文件 */
ErrorCode storage_save_employees(const char *filename, EmployeeManager *manager);

/* 从文件加载职工数据 */
ErrorCode storage_load_employees(const char *filename, EmployeeManager *manager);

/* 导出为CSV格式 */
ErrorCode storage_export_csv(const char *filename, EmployeeManager *manager);

/* ========== 认证存储函数 ========== */

/* 保存用户凭证 */
ErrorCode storage_save_credential(const char *filename, const char *username, 
                                   const char *password);

/* 验证用户凭证 */
Bool storage_verify_credential(const char *filename, const char *username, 
                                const char *password);

#endif /* STORAGE_H */
