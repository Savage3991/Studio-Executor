cmake_minimum_required(VERSION 3.30)
project(StudioExec)

set(CMAKE_CXX_STANDARD 20)

add_library(StudioExec SHARED main.cpp
        src/roblox/engine/engine.h
        src/roblox/taskscheduler/taskscheduler.cpp
        src/roblox/taskscheduler/taskscheduler.h
        src/core/execution/execution.cpp
        src/core/execution/execution.h
        src/core/environment/environment.cpp
        src/core/environment/environment.h
        src/core/environment/library/misc.cpp)

# DEPENDENCIES
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

## LUAU
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/luau")
target_link_libraries(${PROJECT_NAME} PUBLIC Luau.Compiler Luau.Ast Luau.VM Luau.VM.Internals)