set(IR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inst.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vreg.cpp
)

add_library(ir SHARED ${IR_SOURCES})
target_include_directories(ir PRIVATE ${PROJECT_SOURCE_DIR})

add_subdirectory(tests)