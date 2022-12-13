cmake_minimum_required(VERSION 3.10)

project(Anicat)

file(GLOB SOURCE src/*.c lib/*.c)

add_executable(Anicat ${SOURCE})
target_include_directories(Anicat PUBLIC include)
