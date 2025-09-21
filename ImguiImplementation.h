#pragma once
#include "ImguiOpenglBase.h"

class ImguiImplementation final : public ImguiOpenglBase {
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void RenderGui() override;
public:
    explicit ImguiImplementation(const char* windowName) : ImguiOpenglBase(windowName, clear_color) {}
};

class ImguiDemo02 final : public ImguiOpenglBase {
    int counter = 0;
    void RenderGui() override;
public:
    explicit ImguiDemo02(const char* windowName) : ImguiOpenglBase(windowName) {}
};

class ImguiImPlot01 final : public ImguiOpenglBase {
    void RenderGui() override;
public:
    explicit ImguiImPlot01(const char* windowName) : ImguiOpenglBase(windowName) {
        ImPlot::CreateContext();
    }

    ~ImguiImPlot01() override {
        ImPlot::DestroyContext();
    }
};