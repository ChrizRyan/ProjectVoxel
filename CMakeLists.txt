cmake_minimum_required(VERSION 3.20)

project(MyProject)
set(target_name my_project)

include_directories(include)

add_executable(${target_name} 
    src/main.cpp 
    src/Shader.cpp
    src/FileLoader.cpp
    src/Mesh.cpp
    )

include(FetchContent) 

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw 
    GIT_TAG "master"
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Sunday111/glad 
    GIT_TAG "main"
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glad)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG "master"
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(glm)

target_link_libraries(${target_name} PUBLIC glfw glad glm)



