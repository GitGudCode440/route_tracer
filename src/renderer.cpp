#include "renderer.hpp"



void Renderer::render() const
{   
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::defineGeometry() 
{
    //Dealing with vertices
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    printShaderErrors(vertexShader);

    //Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    printShaderErrors(fragmentShader);

    //Linking shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    m_shaderProgram = shaderProgram;

    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Linking Error: " << infoLog << std::endl; 
    }
}

void Renderer::printShaderErrors(GLuint shader) const {
    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        std::cerr << "Shader compile error: " << log << std::endl;
    }
}
