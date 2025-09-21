cmake_minimum_required(VERSION 4.0)
project(Desktop_ImGui02 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Desktop_ImGui02
        main.cpp
        ImguiImplementation.cpp
        ImguiOpenglBase.h
        ImguiImplementation.h
        ImguiImplementation.cpp)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/implot)


# ImGui
file(GLOB IMGUI_SRC
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)


# ImPlot
file (GLOB IMPLOT_SRC
        ${IMPLOT_DIR}/*.cpp
)
add_library(implot STATIC ${IMPLOT_SRC})
target_include_directories(implot PUBLIC ${IMPLOT_DIR} ${IMGUI_DIR})
target_link_libraries(implot PUBLIC imgui)

# Находим GLFW и OpenGL
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Линкуем всё в исполняемый файл
target_link_libraries(Desktop_ImGui02 PRIVATE imgui implot OpenGL::GL glfw)

# Опционально: для отладки
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(Desktop_ImGui02 PRIVATE DEBUG)
endif()


