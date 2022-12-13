set(RUNTIME_SOURCES runtime.cpp runtime_handlers.cpp)

add_library(runtime SHARED ${RUNTIME_SOURCES})

target_link_libraries(runtime ir)

target_include_directories(runtime PRIVATE ${PROJECT_SOURCE_DIR})

add_subdirectory(tests)