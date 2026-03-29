cmake_minimum_required(VERSION 3.20)

set(APP_NAME rt3)
project(${APP_NAME} CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    include
    libs/lodepng
    libs/tinyxml2
    libs/math
)

set(SOURCES
    source/main.cpp
    source/film.cpp
    source/background.cpp
    source/parser.cpp
    source/app.cpp
    libs/lodepng/lodepng.cpp
    libs/tinyxml2/tinyxml2.cpp
)


add_executable(${APP_NAME} ${SOURCES})


if(NOT MSVC)
  target_compile_options(${APP_NAME} PRIVATE
  -Wall
  -Wextra
  -Wshadow
  -Werror
  -Wpedantic
  -g
  -O2
  )
endif()
