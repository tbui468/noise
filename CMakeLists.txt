cmake_minimum_required(VERSION 3.16)

project(Noise VERSION 1.0.0)


if(MSVC)
    add_compile_options(/W4 /w14640 /permissive)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(src)