#ifndef WINDOWER_H
#define WINDOWER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "renderer.hpp"

class Windower {
private:
    GLFWwindow* m_window;
    Renderer& m_renderer;

    int m_windowWidth;
    int m_windowHeight;

    static void m_framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput();
    void resizeViewport(GLFWwindow* window, int width, int height);

public:
    Windower(Renderer& renderer, int windowWidth, int windowHeight);
    void run();
    ~Windower();
};

#endif // WINDOWER_H
