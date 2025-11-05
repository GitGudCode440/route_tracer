#ifndef WINDOWER_H
#define WINDOWER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.hpp"

class Windower {
private:
    GLFWwindow* m_window;
    const Renderer& m_renderer;

    int m_windowWidth;
    int m_windowHeight;

    static void m_framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput();
    void resizeViewport(GLFWwindow* window, int width, int height);

public:
    Windower(const Renderer& renderer, int windowWidth, int windowHeight);
    void run();
    ~Windower();
};

#endif // WINDOWER_H
