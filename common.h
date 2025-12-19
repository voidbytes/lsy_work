#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

/* 兼容不同编译器的 struct pack 写法 */
#if defined(_MSC_VER)
    #define PACK_PUSH __pragma(pack(push, 1))
    #define PACK_POP __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
    #define PACK_PUSH _Pragma("pack(push, 1)")
    #define PACK_POP _Pragma("pack(pop)")
#else
    #define PACK_PUSH
    #define PACK_POP
#endif

/* 布尔类型定义 */
typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

/* 错误码定义 */
typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER = -1,
    ERROR_OUT_OF_MEMORY = -2,
    ERROR_INDEX_OUT_OF_BOUNDS = -3,
    ERROR_INVALID_PARAMETER = -4,
    ERROR_FILE_NOT_FOUND = -5,
    ERROR_FILE_READ_FAILED = -6,
    ERROR_FILE_WRITE_FAILED = -7,
    ERROR_INVALID_FILE = -8,
    ERROR_DATA_CORRUPTION = -9,
    ERROR_AUTH_FAILED = -10,
    ERROR_NOT_FOUND = -11
} ErrorCode;

/* 通用宏定义 */
#define MAX_NAME_LEN 64
#define MAX_DEPT_LEN 64
#define MAX_DATE_LEN 16
#define MAX_PASSWORD_LEN 64
#define MAX_USERNAME_LEN 32

/* 魔数定义 */
#define MAGIC_NUMBER 0x454D5053  /* ASCII: EMPS */
#define FILE_VERSION 1

#endif /* COMMON_H */
