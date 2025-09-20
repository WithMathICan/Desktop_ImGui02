cmake_minimum_required(VERSION 4.0)
project(Desktop_ImGui02 CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Desktop_ImGui02
        main.cpp
        ImguiOpenglBase.cpp
        ImguiOpenglBase.h)

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SRC
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

# Main executable
target_link_libraries(Desktop_ImGui02 PRIVATE imgui glfw opengl32)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Desktop_ImGui02 PRIVATE glfw)