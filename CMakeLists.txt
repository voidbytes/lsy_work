cmake_minimum_required(VERSION 3.10)
project(lsy_work C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# 设置编译器警告标志
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 平台特定配置
if(MSVC)
    # Visual Studio 静态链接标志
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MT")
    # Visual Studio Release 优化标志
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /Ot /GL /Gw /GF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ot /GL /Gw /GF")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")
elif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # macOS Release 优化标志
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -mtune=native -flto -ffast-math -fomit-frame-pointer -funroll-loops -fmerge-all-constants -fdata-sections -ffunction-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native -flto -ffast-math -fomit-frame-pointer -funroll-loops -fmerge-all-constants -fdata-sections -ffunction-sections")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -dead_strip")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # Linux 全静态链接标志
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    # Linux Release 优化标志
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -mtune=native -flto -ffast-math -fomit-frame-pointer -funroll-loops -fmerge-all-constants -fdata-sections -ffunction-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -mtune=native -flto -ffast-math -fomit-frame-pointer -funroll-loops -fmerge-all-constants -fdata-sections -ffunction-sections")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -Wl,--gc-sections -Wl,--strip-all")
endif()

# 代码覆盖率选项
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# 选项：是否构建测试
option(BUILD_TESTS "Build unit tests" ON)

# 确保正确设置 C++ 编译器以支持 .cpp 文件
if(BUILD_TESTS)
    enable_language(CXX)
endif()

# 在跨平台环境中确保测试文件正确包含
if(BUILD_TESTS)
    # 检查测试目录是否存在
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tests")
        message(WARNING "Tests directory not found. Disabling tests.")
        set(BUILD_TESTS OFF)
    endif()
endif()

# 主程序源文件
set(SOURCES
    main.c
    vector.c
    sort.c
    model.c
    storage.c
    view.c
    controller.c
)

# 主程序可执行文件
add_executable(lsy_work ${SOURCES})

# 仅在需要时构建测试
if(BUILD_TESTS)
    # 启用测试
    enable_testing()

    # 下载并配置 Google Test
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # 检查测试文件是否存在
    set(TEST_SOURCES "")
    set(EXPECTED_TEST_FILES
        tests/test_vector.cpp
        tests/test_model.cpp
        tests/test_storage.cpp
        tests/test_sort.cpp
        tests/test_view.cpp
        tests/test_controller.cpp
    )
    
    # 添加源文件
    list(APPEND TEST_SOURCES 
        vector.c
        sort.c
        model.c
        storage.c
        view.c
        controller.c
    )
    
    # 检查每个测试文件是否存在
    foreach(TEST_FILE ${EXPECTED_TEST_FILES})
        if(EXISTS "${CMAKE_SOURCE_DIR}/${TEST_FILE}")
            list(APPEND TEST_SOURCES ${TEST_FILE})
        else()
            message(WARNING "Test file not found: ${TEST_FILE}")
        endif()
    endforeach()
    
    # 只有在找到测试文件时才创建测试可执行文件
    if(TEST_SOURCES)
        # 测试可执行文件
        add_executable(run_tests ${TEST_SOURCES})
        target_link_libraries(run_tests gtest gtest_main)
        target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR})
        
        # 添加测试
        add_test(NAME AllTests COMMAND run_tests)
    else()
        message(WARNING "No test files found. Skipping test build.")
    endif()
endif()