#define GLFW_INCLUDE_NONE

#include <iostream>
#include <Windows.h>
#include <ctime>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"
#include "Game.hpp"
#include "Utils.hpp"

boolean hasWindowReceivedFocusOnce = false;
float oldX = 512 / 2;
float oldY = 512 / 2;


void framebuffer_size_callback(GLFWwindow*, int width, int height);
void mouseCallback(GLFWwindow*, double, double);

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, width);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 1024, "Main window", NULL, NULL);

    if(!window)
    {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD" << std::endl;
        glfwTerminate();
        return 0;
    }

    glViewport(0, 0, 1024, 1024);
    //TODO:: Abstract to window class and to game class
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    Renderer* renderer = Renderer::getInstance();
    
    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        renderer->clear_and_load_shaders(1024, 1024);
        renderer->temp_render_cube(new Cube());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouseCallback(GLFWwindow* win, double x, double y)
{
    //Set mouse location after focus is gained in window for the first time
    if(!hasWindowReceivedFocusOnce)
    {
        oldX = x;
        oldY = y;
        hasWindowReceivedFocusOnce = true;
    }

    //Calculate mouse movement
    float xOff = x - oldX;
    float yOff = -(y - oldY);

    oldX = x;
    oldY = y;

    //Define mouse sensitivity
    float mouseSens = 0.1f;
    xOff *= mouseSens;
    yOff *= mouseSens;

    Camera::getInstance()->rotate(xOff, yOff);
}
