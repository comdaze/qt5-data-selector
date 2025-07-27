# Qt5 Data Selector Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

# 查找Qt5测试模块
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Test)

# 设置测试相关的编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# 获取源文件
file(GLOB_RECURSE MAIN_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# 排除main.cpp，因为测试有自己的main函数
list(FILTER MAIN_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# 单元测试
set(UNIT_TESTS
    test_simple_unit
    test_timewidget_unit
    test_custombutton_unit
    test_devicewidget_unit
)

# 集成测试
set(INTEGRATION_TESTS
    test_mainwindow_integration
)

# 创建单元测试可执行文件
foreach(TEST_NAME ${UNIT_TESTS})
    if(${TEST_NAME} STREQUAL "test_simple_unit")
        # Simple test doesn't need main sources
        add_executable(${TEST_NAME}
            unit/${TEST_NAME}.cpp
        )
    else()
        add_executable(${TEST_NAME}
            unit/${TEST_NAME}.cpp
            ${MAIN_SOURCES}
        )
    endif()
    
    target_link_libraries(${TEST_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Test
    )
    
    # 设置测试属性
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/unit
    )
    
    # 添加到CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # 设置测试环境
    set_tests_properties(${TEST_NAME} PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
        TIMEOUT 30
    )
endforeach()

# 创建集成测试可执行文件
foreach(TEST_NAME ${INTEGRATION_TESTS})
    add_executable(${TEST_NAME}
        integration/${TEST_NAME}.cpp
        ${MAIN_SOURCES}
    )
    
    target_link_libraries(${TEST_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Test
    )
    
    # 设置测试属性
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/integration
    )
    
    # 添加到CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # 设置测试环境
    set_tests_properties(${TEST_NAME} PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
        TIMEOUT 60
    )
endforeach()

# 创建测试套件目标
add_custom_target(run_unit_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test_.*_unit"
    DEPENDS ${UNIT_TESTS}
    COMMENT "Running unit tests"
)

add_custom_target(run_integration_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test_.*_integration"
    DEPENDS ${INTEGRATION_TESTS}
    COMMENT "Running integration tests"
)

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${UNIT_TESTS} ${INTEGRATION_TESTS}
    COMMENT "Running all tests"
)

# 测试覆盖率（如果支持）
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        
        add_custom_target(coverage
            COMMAND lcov --directory . --capture --output-file coverage.info
            COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
            COMMAND lcov --list coverage.info
            COMMAND genhtml -o coverage coverage.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating test coverage report"
        )
    endif()
endif()

# 启用CTest
enable_testing()

# 打印测试信息
message(STATUS "Unit tests: ${UNIT_TESTS}")
message(STATUS "Integration tests: ${INTEGRATION_TESTS}")
message(STATUS "Test output directory: ${CMAKE_BINARY_DIR}/tests")