#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 计算简单的校验和 */
static unsigned int calculate_checksum(const void *data, size_t size) {
    unsigned int sum = 0;
    const unsigned char *bytes = (const unsigned char *)data;
    
    for (size_t i = 0; i < size; i++) {
        sum += bytes[i];
        sum = (sum << 1) | (sum >> 31);  /* 循环左移增加扩散性 */
    }
    
    return sum;
}

/* 保存职工数据到文件 */
ErrorCode storage_save_employees(const char *filename, EmployeeManager *manager) {
    if (filename == NULL || manager == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return ERROR_FILE_WRITE_FAILED;
    }
    
    /* 准备文件头 */
    FileHeader header;
    header.magic = MAGIC_NUMBER;
    header.version = FILE_VERSION;
    header.count = (unsigned int)manager->employees->size;
    header.checksum = 0;  /* 暂时设置为0 */
    
    /* 写入文件头 */
    if (fwrite(&header, sizeof(FileHeader), 1, fp) != 1) {
        fclose(fp);
        return ERROR_FILE_WRITE_FAILED;
    }
    
    /* 写入所有职工数据并计算校验和 */
    unsigned int checksum = 0;
    for (size_t i = 0; i < manager->employees->size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        if (fwrite(emp, sizeof(Employee), 1, fp) != 1) {
            fclose(fp);
            return ERROR_FILE_WRITE_FAILED;
        }
        checksum += calculate_checksum(emp, sizeof(Employee));
    }
    
    /* 更新文件头中的校验和 */
    header.checksum = checksum;
    fseek(fp, 0, SEEK_SET);
    if (fwrite(&header, sizeof(FileHeader), 1, fp) != 1) {
        fclose(fp);
        return ERROR_FILE_WRITE_FAILED;
    }
    
    /* 保存next_id */
    fseek(fp, 0, SEEK_END);
    if (fwrite(&manager->next_id, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return ERROR_FILE_WRITE_FAILED;
    }
    
    fclose(fp);
    return SUCCESS;
}

/* 从文件加载职工数据 */
ErrorCode storage_load_employees(const char *filename, EmployeeManager *manager) {
    if (filename == NULL || manager == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    /* 读取文件头 */
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, fp) != 1) {
        fclose(fp);
        return ERROR_FILE_READ_FAILED;
    }
    
    /* 验证魔数 */
    if (header.magic != MAGIC_NUMBER) {
        fclose(fp);
        return ERROR_INVALID_FILE;
    }
    
    /* 验证版本 */
    if (header.version != FILE_VERSION) {
        fclose(fp);
        return ERROR_INVALID_FILE;
    }
    
    /* 读取所有职工数据并计算校验和 */
    unsigned int checksum = 0;
    for (unsigned int i = 0; i < header.count; i++) {
        Employee *emp = (Employee *)malloc(sizeof(Employee));
        if (emp == NULL) {
            fclose(fp);
            return ERROR_OUT_OF_MEMORY;
        }
        
        if (fread(emp, sizeof(Employee), 1, fp) != 1) {
            free(emp);
            fclose(fp);
            return ERROR_FILE_READ_FAILED;
        }
        
        checksum += calculate_checksum(emp, sizeof(Employee));
        
        ErrorCode err = vector_push_back(manager->employees, emp);
        if (err != SUCCESS) {
            free(emp);
            fclose(fp);
            return err;
        }
    }
    
    /* 验证校验和 */
    if (checksum != header.checksum) {
        fclose(fp);
        return ERROR_DATA_CORRUPTION;
    }
    
    /* 读取next_id */
    if (fread(&manager->next_id, sizeof(int), 1, fp) != 1) {
        /* 如果没有next_id,使用默认值 */
        manager->next_id = 1001;
        if (manager->employees->size > 0) {
            /* 找到最大的ID并加1 */
            int max_id = 1000;
            for (size_t i = 0; i < manager->employees->size; i++) {
                Employee *emp = (Employee *)manager->employees->data[i];
                if (emp->id > max_id) {
                    max_id = emp->id;
                }
            }
            manager->next_id = max_id + 1;
        }
    }
    
    fclose(fp);
    return SUCCESS;
}

/* 导出为CSV格式 */
ErrorCode storage_export_csv(const char *filename, EmployeeManager *manager) {
    if (filename == NULL || manager == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return ERROR_FILE_WRITE_FAILED;
    }
    
    /* 写入CSV头 */
    fprintf(fp, "工号,姓名,部门,出勤日期,出勤天数\n");
    
    /* 写入所有职工数据 */
    for (size_t i = 0; i < manager->employees->size; i++) {
        Employee *emp = (Employee *)manager->employees->data[i];
        fprintf(fp, "%d,%s,%s,%s,%d\n",
                emp->id, emp->name, emp->department,
                emp->attend_date, emp->attend_days);
    }
    
    fclose(fp);
    return SUCCESS;
}

/* 简单的字符串哈希函数(用于密码加密) */
static unsigned int simple_hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  /* hash * 33 + c */
    }
    
    return hash;
}

/* 保存用户凭证 */
ErrorCode storage_save_credential(const char *filename, const char *username,
                                   const char *password) {
    if (filename == NULL || username == NULL || password == NULL) {
        return ERROR_NULL_POINTER;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return ERROR_FILE_WRITE_FAILED;
    }
    
    UserCredential cred;
    strncpy(cred.username, username, MAX_USERNAME_LEN - 1);
    cred.username[MAX_USERNAME_LEN - 1] = '\0';
    
    /* 简单加密:保存密码的哈希值 */
    unsigned int hash = simple_hash(password);
    snprintf(cred.password, MAX_PASSWORD_LEN, "%u", hash);
    
    if (fwrite(&cred, sizeof(UserCredential), 1, fp) != 1) {
        fclose(fp);
        return ERROR_FILE_WRITE_FAILED;
    }
    
    fclose(fp);
    return SUCCESS;
}

/* 验证用户凭证 */
Bool storage_verify_credential(const char *filename, const char *username,
                                const char *password) {
    if (filename == NULL || username == NULL || password == NULL) {
        return FALSE;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return FALSE;
    }
    
    UserCredential cred;
    if (fread(&cred, sizeof(UserCredential), 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }
    
    fclose(fp);
    
    /* 验证用户名 */
    if (strcmp(cred.username, username) != 0) {
        return FALSE;
    }
    
    /* 验证密码 */
    unsigned int hash = simple_hash(password);
    char hash_str[MAX_PASSWORD_LEN];
    snprintf(hash_str, MAX_PASSWORD_LEN, "%u", hash);
    
    if (strcmp(cred.password, hash_str) != 0) {
        return FALSE;
    }
    
    return TRUE;
}
