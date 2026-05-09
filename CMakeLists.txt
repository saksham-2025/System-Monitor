cmake_minimum_required(VERSION 3.10)

project(SystemMonitor)

include_directories(include)

add_executable(monitor
    src/main.cpp
    src/process.cpp
    src/system.cpp
    src/renderer.cpp
)