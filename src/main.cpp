#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "map_data.hpp"
#include "a_star.hpp"

#include "windower.hpp"
#include "renderer.hpp"


int main(void)
{  
    // parseMap();
    // aStar();
    
    Renderer renderer;
    renderer.setVertices({
        -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, 1.0f,
        0.0f, 0.5f, 1.0f
    });
   

    Windower windower(renderer, 800, 640);
    windower.run();

}