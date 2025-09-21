#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class GLFWInitializer {
public:
    GLFWInitializer() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    ~GLFWInitializer() {
        glfwTerminate();
    }

    // Удаляем копирование и перемещение, чтобы избежать дублирования инициализации
    GLFWInitializer(const GLFWInitializer&) = delete;
    GLFWInitializer& operator=(const GLFWInitializer&) = delete;
    GLFWInitializer(GLFWInitializer&&) = delete;
    GLFWInitializer& operator=(GLFWInitializer&&) = delete;
};

using GLFW_windowPtr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;

class ImguiOpenglBase {
    static void glfw_error_callback(int error, const char* description) {
        std::cerr << "GLFW Error " << error << " : " << description << std::endl;
    }
    GLFW_windowPtr window;
    GLFWInitializer glfw_init;

protected:
    virtual void RenderGui() = 0;
    ImGuiIO io;
    float main_scale;
    const char * Window_Name;
public:
    static void UpdateBackgroundColor(const ImVec4 &clear_color) {
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    explicit ImguiOpenglBase(const char* windowName)
        : window(nullptr, glfwDestroyWindow),
          Window_Name(windowName) {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Получение масштаба после glfwInit
        if (GLFWmonitor* monitor = glfwGetPrimaryMonitor()) {
            main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);
        } else {
            std::cerr << "Warning: Failed to get primary monitor, using scale 1.0" << std::endl;
            main_scale = 1.0f;
        }

        // Создание окна и присвоение unique_ptr
        GLFWwindow* window_raw = glfwCreateWindow(static_cast<int>(1280 * main_scale), static_cast<int>(800 * main_scale), Window_Name, nullptr, nullptr);
        if (window_raw == nullptr) throw std::runtime_error("Failed to create GLFW window.");
        window.reset(window_raw);

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        // Setup Dear ImGui style
        ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        style.FontSizeBase = 17.0f;
        io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\tahoma.ttf)");
    }

    ImguiOpenglBase(const ImguiOpenglBase&) = delete;
    ImguiOpenglBase& operator=(const ImguiOpenglBase&) = delete;
    ImguiOpenglBase(ImguiOpenglBase&&) = delete;
    ImguiOpenglBase& operator=(ImguiOpenglBase&&) = delete;

    void Show() {
        while (!glfwWindowShouldClose(window.get())) {
            glfwPollEvents();
            if (glfwGetWindowAttrib(window.get(), GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(100);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            RenderGui();

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window.get(), &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window.get());
        }
    }

    virtual ~ImguiOpenglBase() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        //glfwTerminate();
    }
};