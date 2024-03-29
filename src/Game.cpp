#include "World.hpp"
#include "Game.hpp"

unsigned int WINDOW_WIDTH = 1024; 
unsigned int WINDOW_HEIGHT = 1024;

Game* Game::_instance{nullptr};

void Game::init(GLFWwindow* new_window)
{
    //world = new World();
    camera = Camera::getInstance();
    renderer = Renderer::getInstance();
    window = new_window;
}


Game* Game::getInstance(GLFWwindow* window)
{
    if(_instance == nullptr)
    {
        _instance = new Game(window);
    }
    return _instance;
}

void Game::update()
{
    //Call update if necessary
    processInput();
}

void Game::processInput()
{
    float speed = 5.0f * renderer->dTime;
    bool buttonPressed = false;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        buttonPressed = true;
        camera->forward(speed);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        buttonPressed = true;
        camera->reverse(speed);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        buttonPressed = true;
        camera->moveLeft(speed);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        buttonPressed = true;
        camera->moveRight(speed);
    }
}
