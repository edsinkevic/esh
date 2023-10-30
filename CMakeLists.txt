cmake_minimum_required(VERSION 3.26)
project(esh C)
set(CMAKE_C_STANDARD 23)

set(SRC "src")

add_executable(esh ${SRC}/main.c ${SRC}/esh_array.c
        src/esh_proc.h
        src/esh_cmd.c
        src/esh_control.c
        src/esh_control.c
        src/esh_kill.h
        src/esh_kill.c
        src/esh_fg.h
        src/esh_fg.c
        src/esh_proc.c
        src/esh_utils.c
)