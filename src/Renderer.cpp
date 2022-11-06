#define GLFW_INCLUDE_NONE

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include <algorithm>
#include "blocks/Block.hpp"
#include "Camera.hpp"

Renderer* Renderer::_instance{nullptr};

Renderer::Renderer()
{
    init();
}

Renderer* Renderer::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new Renderer();
    }
    return _instance;
}

void Renderer::init()
{
    //Load shader programs...
    sp = new ShaderProgram("../shaders/VertexShader.vshader", "../shaders/FragmentShader.fshader");
    txAtlas = std::make_unique<TextureAtlas>();
}

void Renderer::clear_and_load_shaders(unsigned int wWidth, unsigned int wHeight)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sp->activate();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)wWidth/(float)wHeight, 0.1f, 100.0f);
    sp->setMat4("projection", projection);
    sp->setMat4("view", Camera::getInstance()->getView());
}

void Renderer::render_chunks()
{
    for(const auto& chunk_renderer : chunk_renderers)
    {
        chunk_renderer->render_chunk(txAtlas.get(), sp);
    }
}

void Renderer::tick()
{
    curFrame = glfwGetTime();
    dTime = curFrame - lastFrame;
    framerate = 1/dTime;
    lastFrame = curFrame;
}
