set(IR_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/irbuilder_test.cpp
)

set(GTEST_INCLUDE_DIR third-party/googletest/googletest/include)

add_executable(ir_tests ${IR_TEST_SOURCES})
target_link_libraries(ir_tests ir gtest pthread)
target_include_directories(ir_tests PRIVATE ${PROJECT_SOURCE_DIR} SHARED ${GTEST_INCLUDE_DIR})