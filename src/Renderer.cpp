#define GLFW_INCLUDE_NONE

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "blocks/Cube.hpp"
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
    sp = new ShaderProgram("../shaders/VertexShader.vshader", "../shaders/FragmentShader.fshader");

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao); 
}

void Renderer::temp_add_data(std::vector<float> data, unsigned int textureBuf)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, textureBuf);

    //Unbind buffers, data is bound to vao now
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Renderer::temp_render_cube(Cube* cube)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cube->getPosition()); 

    sp->setMat4("model", model);


    temp_add_data(cube->getData(), cube->getTextureBuffer());

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, cube->getActiveFacesCount() * 6);
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

void Renderer::prepare_frame()
{
    curFrame = glfwGetTime();
    dTime = curFrame - lastFrame;
    framerate = 1/dTime;
    lastFrame = curFrame;

    //Check if window size has changed, ready perspective and stuff (Unless OpenGL does it already, then just call those functions here, unless they're callbacks)
    //TODO:: Texture manager should store the textures in a hashmap
}
