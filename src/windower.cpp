#include <iostream>

#include "windower.hpp"

Windower::Windower(Renderer& renderer, int windowWidth, int windowHeight)
    : m_renderer(renderer), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    if (!glfwInit()) {
        std::cout << "GLFW not initialized!" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Hello World", nullptr, nullptr);
    if (!m_window) {
        std::cout << "Failed to initialize GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glfwSetFramebufferSizeCallback(m_window, m_framebufferSizeCallback);
    glfwSetWindowUserPointer(m_window, this);

    m_renderer.defineGeometry();
}

void Windower::run() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();
        m_renderer.render();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Windower::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Windower::resizeViewport(GLFWwindow* window, int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    glViewport(0, 0, m_windowWidth, m_windowHeight);
}

void Windower::m_framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Windower* win = reinterpret_cast<Windower*>(glfwGetWindowUserPointer(window));
    if (win) win->resizeViewport(window, width, height);
}

Windower::~Windower() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
