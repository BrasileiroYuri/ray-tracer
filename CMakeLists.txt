cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SRC source)
set(INC include)
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

add_library(tinyxml2 libs/tinyxml2/tinyxml2.cpp)
target_include_directories(tinyxml2 PUBLIC libs/tinyxml2)
target_link_libraries(${APP_NAME} tinyxml2)

add_library(lodepng libs/lodepng/lodepng.cpp)
target_include_directories(lodepng PUBLIC libs/lodepng)
target_link_libraries(${APP_NAME} lodepng)

add_library(math libs/math/vec3.cpp)
target_include_directories(math PUBLIC libs/math)
target_link_libraries(${APP_NAME} math)
