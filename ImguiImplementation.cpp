#include "ImguiImplementation.h"
#include "implot/implot.h"

void ImguiImplementation::RenderGui() {

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));

        if (ImGui::Button("Button")) counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        const ImGuiIO& io = ImGui::GetIO();
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

    setBackgroundColor(clear_color);
}

void ImguiDemo02::RenderGui() {
    ImGui::Begin("Demo Window 02!");
    ImGui::Text("Press button to change counter");
    ImGui::Text("Counter = %d", counter);
    if (ImGui::Button("Increase Counter")) ++counter;
    ImGui::SameLine();
    if (counter > 0) {
        if (ImGui::Button("Decrease Counter")) --counter;
    } else {
        ImGui::BeginDisabled();
        ImGui::Button("Decrease Counter");
        ImGui::EndDisabled();
    }
    ImGui::End();
}

void ImguiImPlot01::RenderGui() {
    ImGui::Begin("ImGui & ImPlot");
    ImGui::Text("This is plot");
    if (ImPlot::BeginPlot("Simple Plot", ImVec2(-1, -1))) {  // -1 означает ширину окна, 150 — высота
        float x[] = {0.0f, 1.0f, 2.0f};
        float y[] = {1.0f, 2.0f, -5.0f};
        ImPlot::SetupAxes("X-axis", "Y-axis");
        ImPlot::PlotLine("Data", x, y, 3);  // Рисуем линию через точки
        ImPlot::EndPlot();
    }
    ImGui::End();
}

