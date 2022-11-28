set(IR_SOURCES )

add_library(ir SHARED ${IR_SOURCES})

target_include_directories(ir ${PROJECT_SOURCE_DIR})
