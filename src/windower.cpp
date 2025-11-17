#include "windower.hpp"
#include <iostream>
#include "imgui_panel.hpp"


// Modern Dark Theme Function
// --------------------------
void ApplyModernDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 6.0f;

    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;

    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(12, 12);

    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.35f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.45f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.55f, 1.0f);

    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.30f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.40f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.50f, 1.0f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.28f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.32f, 1.0f);
}


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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ApplyModernDarkTheme();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);          
    ImGui_ImplOpenGL3_Init();

    m_renderer.defineGeometry();
}

void Windower::run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        processInput();        

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


     ShowRouteTracerPanel(*this);


        m_renderer.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(m_window);
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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}
