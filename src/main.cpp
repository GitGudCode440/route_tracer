#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "map_data.hpp"
#include "a_star.hpp"

#include "windower.hpp"
#include "renderer.hpp"


int main(void)
{  
    Renderer renderer;
    Windower windower(renderer, 800, 640);
    windower.run();

}