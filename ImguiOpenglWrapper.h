#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

struct background_color {
    float r, g, b, a;
    background_color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
};

class ImguiOpenglWrapper {
    background_color Background_Color;


    static void glfw_error_callback(int error, const char* description) {
        std::cerr << "GLFW Error " << error << " : " << description << std::endl;
    }

    void UpdateBackgroundColor() const {
        glClearColor(Background_Color.r, Background_Color.g, Background_Color.b, Background_Color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
public:
    GLFWwindow* window;
    float main_scale;
    ImguiOpenglWrapper(const char * windowTitle, background_color color) : Background_Color(color) {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

        const char*  glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
        window = glfwCreateWindow(static_cast<int>(1280 * main_scale), static_cast<int>(800 * main_scale), windowTitle, nullptr, nullptr);
        if (window == nullptr) throw std::runtime_error("Failed to open GLFW window.");
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        UpdateBackgroundColor();
    }

    void SetBackgroundColor(background_color color) {
        Background_Color = color;
        UpdateBackgroundColor();
    }

    ~ImguiOpenglWrapper() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }
};
