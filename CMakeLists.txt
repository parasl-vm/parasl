cmake_minimum_required(VERSION 3.16)
project(ParaSL)

set(CMAKE_CXX_STANDARD 17)

function(add_git_submodule dir)
    # add a Git submodule directory to CMake, assuming the
    # Git submodule directory is a CMake project.
    #
    # Usage: in CMakeLists.txt
    #
    # include(AddGitSubmodule.cmake)
    # add_git_submodule(mysubmod_dir)

    find_package(Git REQUIRED)

    if(NOT EXISTS ${dir}/CMakeLists.txt)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()

    add_subdirectory(${dir})

endfunction(add_git_submodule)

find_package(BISON)
find_package(FLEX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDEBUG)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add_subdirectory(lexer)
# add_subdirectory(runtime)
# add_subdirectory(parser)
add_subdirectory(ir)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_git_submodule(third_party/googletest)
    # add_subdirectory(tests)
endif()
