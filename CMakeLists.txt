cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(BODYFRIEND_GPB)

add_definitions(-DSTM32F0XX)
add_subdirectory(src/uart)
add_subdirectory(src)


 
