cmake_minimum_required(VERSION 2.8)

project(StringList)
set(SOURCES
    "src/StringList.c")
include_directories(include)
add_executable(${PROJECT_NAME} ${SOURCES} "main.c")
