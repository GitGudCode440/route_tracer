#ifndef RENDERER
#define RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <iostream>



class Renderer {
private:

    float m_vertices[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    const std::string vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const std::string fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
        "}\0";

    GLuint m_shaderProgram;

    GLuint createShader(GLenum type, const std::string& source);

    GLuint linkShadersIntoProgram(const std::vector<GLuint>&& shaders);
    

public:

    Renderer() = default;

    ~Renderer() = default;

    

    void render() const;

    void defineGeometry();
};

#endif