#ifndef RENDERER
#define RENDERER

#include <GLFW/glfw3.h>


class Renderer {
public:

    Renderer() = default;

    ~Renderer() = default;

    void render() const;

    void defineGeometry() const;
};

#endif