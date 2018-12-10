cmake_minimum_required(VERSION 3.10)
project(Cocurrent)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD  11)

add_executable(SOI_3 src/main.cpp src/terminal-stream.cpp include/terminal-stream.h include/fdstream.h src/buffer.cpp include/buffer.h include/car-type.h src/car-type.cpp)
