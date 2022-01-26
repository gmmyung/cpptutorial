cmake_minimum_required(VERSION 3.22.1)

project(HELLO_WORLD)

add_executable(${PROJECT_NAME} hello_world.cpp)

add_subdirectory(Adder)

target_include_directories(${PROJECT_NAME} PUBLIC Adder)

target_link_directories(${PROJECT_NAME} PRIVATE Adder)

target_link_libraries(${PROJECT_NAME} adder)