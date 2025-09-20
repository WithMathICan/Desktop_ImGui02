#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImguiOpenglWrapper.h"

class ImGuiImplementation {
    ImguiOpenglWrapper Imgui_OpenglWrapper;
    ImGuiIO io;

    bool show_demo_window = true;
    bool show_another_window = false;
    background_color clear_color{0.45f, 0.55f, 0.60f, 1.00f};

    void Render() {
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color)); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        Imgui_OpenglWrapper.SetBackgroundColor(clear_color);
    }
public:
    explicit ImGuiImplementation(const ImguiOpenglWrapper & imguiWrapper) : Imgui_OpenglWrapper(imguiWrapper) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        ImGui::StyleColorsLight();

        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(imguiWrapper.main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = imguiWrapper.main_scale;
        style.FontSizeBase = 17.0f;
        io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\tahoma.ttf)");
    }

    void Show() {
        while (!glfwWindowShouldClose(Imgui_OpenglWrapper.window)) {
            glfwPollEvents();
            if (glfwGetWindowAttrib(Imgui_OpenglWrapper.window, GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(100);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Render();

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(Imgui_OpenglWrapper.window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(Imgui_OpenglWrapper.window);
        }
    }
};

int main() {
    ImguiOpenglWrapper imgui_opengl_wrapper{"Window Name", background_color(0,0,0,1)};
    ImGuiImplementation im_gui_implementation{imgui_opengl_wrapper};
    im_gui_implementation.Show();
}
