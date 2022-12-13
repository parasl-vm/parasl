set(RUNTIME_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/runtime_test1.cpp
)

set(GTEST_INCLUDE_DIR third-party/googletest/googletest/include)

add_executable(runtime_tests ${RUNTIME_TEST_SOURCES})
target_link_libraries(runtime_tests runtime gtest pthread)
target_include_directories(runtime_tests PRIVATE ${PROJECT_SOURCE_DIR} SHARED ${GTEST_INCLUDE_DIR})