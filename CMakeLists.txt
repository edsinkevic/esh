cmake_minimum_required(VERSION 3.26)
project(esh C)
set(CMAKE_C_STANDARD 23)

set(SRC "src")

add_executable(esh ${SRC}/main.c ${SRC}/esh_tokens.c ${SRC}/esh_bg.c )