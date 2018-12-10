cmake_minimum_required(VERSION 3.10)
project(Cocurrent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD  11)

add_executable(Cocurrent src/main.cpp src/terminal-stream.cpp include/terminal-stream.h include/fdstream.h src/buffer.cpp include/buffer.h include/car-type.h src/car-type.cpp)
target_link_libraries(Cocurrent Threads::Threads)