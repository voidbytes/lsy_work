cmake_minimum_required(VERSION 3.10)
project(lsy_work C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 代码覆盖率选项
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# 选项：是否构建测试
option(BUILD_TESTS "Build unit tests" ON)

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

    # 测试文件
    set(TEST_SOURCES
        tests/test_vector.cpp
        tests/test_model.cpp
        tests/test_storage.cpp
        tests/test_sort.cpp
        tests/test_view.cpp
        tests/test_controller.cpp
        vector.c
        sort.c
        model.c
        storage.c
        view.c
        controller.c
    )

    # 测试可执行文件
    add_executable(run_tests ${TEST_SOURCES})
    target_link_libraries(run_tests gtest gtest_main)
    target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR})

    # 添加测试
    add_test(NAME AllTests COMMAND run_tests)
endif()
