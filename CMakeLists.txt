cmake_minimum_required(VERSION 3.7.2)
project(topk_words CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(topk_words topk_words.cpp)

if (MSVC)
    target_compile_options(topk_words PUBLIC /W4)
else ()
    target_compile_options(topk_words PUBLIC -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
endif ()