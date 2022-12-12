set(RUNTIME_SOURCES runtime.cpp runtime_handlers.cpp)

add_library(runtime SHARED ${RUNTIME_SOURCES})

target_include_directories(runtime ${PROJECT_SOURCE_DIR})
