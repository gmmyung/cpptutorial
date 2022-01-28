cmake_minimum_required(VERSION 3.22.1)

project(HELLOWORLD VERSION 1.0)

configure_file(HELLOWORLDConfig.h.in HELLOWORLDConfig.h)

add_executable(${PROJECT_NAME} hello_world.cpp)


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

add_subdirectory(external/glfw)

add_subdirectory(external/glad)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/glfw/include
    PUBLIC external/glad/include
    PUBLIC external/stb
    PUBLIC include)

target_link_directories(${PROJECT_NAME} 
    PRIVATE external/glfw/src
    PRIVATE external/glad/src)

target_link_libraries(${PROJECT_NAME} glfw glad)