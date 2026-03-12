cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SRC source)
set(inc include)
set(APP_NAME ray_tracer)

project(${APP_NAME} CXX)

add_executable(${APP_NAME}
  ${SRC}/background.cpp
  ${SRC}/film.cpp
  ${SRC}/parser.cpp
  ${SRC}/param_set.cpp
  ${SRC}/main.cpp
)

target_include_directories(${APP_NAME} PRIVATE ${INC})

target_compile_options(${APP_NAME} PRIVATE
 -Wpedantic
 -Wshadow
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -march=native
)

