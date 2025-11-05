#include "renderer.hpp"


void Renderer::render() const
{
    glClearColor(1.0, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::defineGeometry() const
{
    
}
