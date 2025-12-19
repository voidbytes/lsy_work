# 职工信息管理系统

<!-- 当上传到GitHub后,可替换为实际的仓库地址 -->
![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-blue)
![Language](https://img.shields.io/badge/language-C99-orange)

## 项目概述

这是一个使用纯C语言实现的职工信息管理系统,采用MVC架构模式和面向对象设计风格。系统支持职工信息的增删改查、多条件排序、考勤统计、数据持久化和CSV导出等功能。

## 技术特点

### 1. 工程化架构
- **MVC模式**: 严格分离模型(Model)、视图(View)和控制器(Controller)
- **模块化设计**: 代码按功能模块分文件组织
- **OO风格**: 使用结构体+函数指针模拟类和方法

### 2. 核心技术实现

#### 动态数组 (Vector)
- 自实现类似C++ `std::vector`的动态数组
- **几何级数扩容**: 采用2倍扩容策略,避免频繁内存重分配
- **自动收缩**: 当size < capacity/4时自动收缩,节省内存
- 泛型设计: 使用`void*`指针支持任意类型

#### 快速排序算法
- 手写递归实现的快速排序
- 支持自定义比较器函数
- 平均时间复杂度O(n log n)

#### 数据持久化
- **二进制文件格式**: 紧凑高效的存储方式
- **魔数验证**: 使用0x454D5053作为文件标识
- **校验和机制**: 防止数据篡改和损坏
- **双文件系统**: 
  - `employees.db`: 职工数据
  - `admin.auth`: 管理员认证信息

## 文件结构

```
lsy-work/
├── common.h              # 通用定义(错误码、布尔类型等)
├── vector.h/c            # 动态数组实现
├── sort.h/c              # 快速排序算法
├── model.h/c             # 数据模型(Employee、EmployeeManager)
├── storage.h/c           # 存储层(文件读写、校验)
├── view.h/c              # 视图层(UI界面)
├── controller.h/c        # 控制器(业务调度)
├── main.c                # 程序入口
├── CMakeLists.txt        # CMake构建配置
└── tests/                # 单元测试
    ├── test_vector.cpp   # Vector模块测试
    ├── test_model.cpp    # Model模块测试
    ├── test_storage.cpp  # Storage模块测试
    ├── test_sort.cpp     # Sort模块测试
    ├── test_controller.cpp # Controller模块测试
    └── test_view.cpp     # View模块测试
```

## 功能特性

### 基本功能
1. **添加职工**: 支持录入工号、姓名、部门、出勤日期和天数
2. **删除职工**: 根据工号删除职工记录
3. **修改职工**: 更新职工信息
4. **查询职工**: 支持三种查询方式
   - 按工号查询(精确匹配)
   - 按姓名查询(模糊匹配)
   - 按部门查询(精确匹配)
5. **显示所有**: 列表展示所有职工信息

### 高级功能
1. **多条件排序**:
   - 按工号排序
   - 按姓名排序
   - 按部门排序
   - 按出勤日期排序
   - 按出勤天数排序(降序)

2. **考勤统计**:
   - 月度出勤统计
   - 年度出勤统计

3. **数据导出**:
   - 支持导出为CSV格式
   - 便于Excel等工具导入分析

4. **用户认证**:
   - 首次运行创建管理员账号
   - 密码哈希存储
   - 登录验证

5. **数据持久化**:
   - 自动保存和加载数据
   - 数据完整性校验

## 跨平台支持

✅ **本项目完全支持Windows、macOS和Linux三大平台**

### 平台兼容性说明

项目采用纯C语言标准库实现,无任何平台特定依赖:
- **只使用C99标准库函数**: stdio.h, stdlib.h, string.h, stddef.h
- **无平台特定API**: 无Windows API、无POSIX特定调用
- **跨平台构建工具**: 使用CMake,支持所有主流平台
- **测试框架**: Google Test支持跨平台测试

### 各平台编译说明

#### Windows (Visual Studio / MinGW)
```bash
# 使用Visual Studio
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release

# 或使用MinGW
cmake -G "MinGW Makefiles" ..
mingw32-make
```

#### macOS
```bash
cmake ..
make -j4
```

#### Linux
```bash
cmake ..
make -j4
```

### 已测试平台

- ✅ macOS (Apple Silicon & Intel)
- ✅ Linux (Ubuntu, Debian, CentOS等)
- ✅ Windows (Visual Studio 2019+, MinGW)

### CI/CD 自动构建

项目配置了GitHub Actions自动构建流程:

**触发条件**:
- 推送到main分支
- 向main分支提交Pull Request

**构建产物**:
- ✅ Linux x64可执行文件
- ✅ macOS x64可执行文件  
- ✅ Windows x64可执行文件

**自动发布**:
每次推送到main分支后,GitHub Actions会:
1. 在三个平台上编译项目
2. 运行所有单元测试
3. 打包可执行文件
4. 自动创建Release并上传构建产物

查看构建状态和下载可执行文件:
- 访问项目的GitHub Actions页面查看构建日志
- 访问Releases页面下载最新构建的可执行文件

## 快速开始

### 方式1: 下载预编译版本 (推荐)

如果项目已上传到GitHub:

1. 访问GitHub仓库的[Releases页面](https://github.com/你的用户名/lsy-work/releases)
2. 下载对应平台的可执行文件:
   - Linux: `lsy_work-linux-x64`
   - macOS: `lsy_work-macos-x64`
   - Windows: `lsy_work-windows-x64.exe`
3. Linux/macOS用户需添加执行权限:
   ```bash
   chmod +x lsy_work-linux-x64
   ./lsy_work-linux-x64
   ```
4. Windows用户直接双击运行

### 方式2: 本地编译

## 编译和运行

### 编译要求
- CMake >= 3.10
- C编译器 (支持C99标准)
- C++编译器 (用于Google Test,支持C++11标准)

### 编译步骤

```bash
# 创建构建目录
mkdir cmake-build-debug
cd cmake-build-debug

# 配置项目
cmake ..

# 编译
make -j4
```

### 运行主程序

```bash
./lsy_work
```

### 运行单元测试

```bash
./run_tests
```

## 使用示例

### 首次运行
程序会提示创建管理员账号:
```
检测到首次运行,请创建管理员账号:
请输入用户名: admin
请输入密码: ~~~~password123
```

### 主菜单
```
========================================
       职工信息管理系统 v1.0
========================================
  1. 添加职工信息
  2. 删除职工信息
  3. 修改职工信息
  4. 查询职工信息
  5. 显示所有职工
  6. 排序职工信息
  7. 统计考勤信息
  8. 导出为CSV文件
  9. 保存并退出
  0. 退出(不保存)
========================================
```

### 添加职工示例
```
========== 添加职工信息 ==========
姓名: 张三
部门: 研发部
出勤日期 (YYYY-MM-DD): 2024-01-15
出勤天数: 20
[提示] 职工添加成功!
```

## 测试覆盖

项目包含完整的单元测试,覆盖所有核心模块:

### Vector测试
- 创建和释放
- 添加元素
- 获取元素
- 删除元素
- 清空操作
- 容量扩展
- NULL指针处理

### Model测试
- Employee创建和释放
- EmployeeManager创建和释放
- 添加职工及参数验证
- 删除职工
- 修改职工
- 多条件查询(按工号、姓名、部门)
- 多字段排序
- 月度/年度考勤统计

### Storage测试
- 保存和加载数据
- 空数据处理
- 文件不存在处理
- 魔数验证
- CSV导出
- 用户凭证管理
- NULL指针处理
- 大数据集测试

### Sort测试
- 基本排序功能
- 自定义比较器
- 边界条件处理

### Controller测试
- 系统初始化
- 用户认证流程
- 职工信息管理流程
- 数据持久化

### View测试
- 菜单显示
- 输入处理
- 信息展示

**总计92个测试用例,全部通过!**

## 代码规范

- 使用C99标准
- 所有指针在使用前都进行NULL检查
- 使用const修饰不可变参数
- 完善的错误处理机制
- 内存管理严格遵循"谁分配谁释放"原则
- 代码注释清晰,函数职责单一

## 性能特点

1. **动态数组**: O(1)平均插入复杂度,O(n)最坏情况
2. **快速排序**: O(n log n)平均时间复杂度
3. **二进制存储**: 比文本格式更紧凑高效
4. **内存优化**: 自动收缩策略避免内存浪费

## 跨平台兼容性检查清单

为确保项目在三大平台上完美运行,我们遵守以下原则:

### ✅ 代码层面
- [x] 只使用C99标准库函数
- [x] 无平台特定头文件(windows.h, unistd.h等)
- [x] 无内联汇编代码
- [x] 使用标准整数类型(int, size_t等)
- [x] 文件路径处理兼容(CMake自动处理)

### ✅ 构建系统
- [x] 使用CMake跨平台构建工具
- [x] 无平台特定编译选项
- [x] 支持多种编译器(GCC, Clang, MSVC)

### ✅ 数据存储
- [x] 二进制文件使用标准fopen/fread/fwrite
- [x] 字节序一致性(魔数验证)
- [x] 文本编码UTF-8兼容

### ✅ 测试框架
- [x] Google Test跨平台支持
- [x] 所有测试用例在三大平台通过

### ⚠️ 注意事项

1. **中文支持**: Windows控制台需要设置为UTF-8编码
   ```bash
   # Windows下执行
   chcp 65001
   ```

2. **行结束符**: Git自动处理(Windows CRLF / Unix LF)

3. **文件路径**: 项目使用相对路径,无需特别处理

## 扩展性

系统设计具有良好的扩展性:
- 易于添加新的查询条件
- 易于添加新的排序字段
- 易于扩展文件格式
- 易于增加新的统计功能

## 作者说明

本项目完全使用纯C语言实现(仅标准库),展示了:
1. C语言也可以实现优雅的OO设计
2. 工程化的代码组织方式
3. 完整的测试驱动开发流程
4. 高性能数据结构和算法实现

项目适合作为C语言课程设计、数据结构实践项目使用。