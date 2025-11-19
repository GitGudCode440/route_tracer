#ifndef RENDERER
#define RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>



class Renderer {
private:

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    GLuint m_VAO;
    GLenum m_drawMode;
    std::vector<size_t> m_segmentOffsets;
    std::vector<size_t> m_segmentLengths;

    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    GLuint m_shaderProgram;
    GLint m_uOffsetLoc = -1;
    GLint m_uScaleLoc = -1;
    GLint m_uAspectLoc = -1;
    float m_camOffsetX = 0.0f;
    float m_camOffsetY = 0.0f;
    float m_camScale = 1.0f;
    int m_viewportWidth = 800;
    int m_viewportHeight = 640;

    void readShader(const std::string& filepath);
    GLuint createShader(GLenum type, const std::string& source);
    GLuint linkShadersIntoProgram(const std::vector<GLuint>&& shaders);
    

public:

    Renderer();
    ~Renderer() = default;

    void setDrawMode(GLenum mode) { m_drawMode = mode; }


    void setCamera(float ox, float oy, float scale);
    void setViewportSize(int width, int height);

    void setSegmentInfo(const std::vector<size_t>& offsets, const std::vector<size_t>& lengths) {
        m_segmentOffsets = offsets;
        m_segmentLengths = lengths;
    }
    void setVertices(const std::vector<float>& arr) { m_vertices = arr; }
    void setIndices(const std::vector<unsigned int>& arr) { m_indices = arr; }

    
    void render() const;
    void defineGeometry();
};

#endif