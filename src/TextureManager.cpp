#include "TextureManager.hpp"
#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <mutex>

std::mutex TextureManager::_mutex;
TextureManager* TextureManager::_instance{nullptr};

TextureManager::TextureManager()
{

}

TextureManager* TextureManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new TextureManager();
    }
    return _instance;
}

unsigned int TextureManager::createTexture(std::string texture_path)
{
    std::lock_guard<std::mutex> guard(_mutex);
    std::cout << "Creating texture for a cube" << std::endl;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    std::cout << "Bound" << std::endl;

    //Configure texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "Configured" << std::endl;
    //Load textures
    int width, height, nrChannels;
    unsigned char* dirt_data;

    std::cout << "Loading dirt data" << std::endl;
    dirt_data = stbi_load(("..\\textures\\" + texture_path).c_str(), &width, &height, &nrChannels, 0);

    if(!dirt_data)
    {
        std::cout << "Failed to load the texture" << std::endl;
        return -1;
    }

    std::cout << "File loaded" << std::endl;

    //Generate texture TODO:: Do this in cube class instead of here 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dirt_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    std::cout << "Freeing" << std::endl;
    stbi_image_free(dirt_data);

    std::cout << "Generated" << std::endl;

    std::cout << "[4] " << texture << std::endl;
    return texture;
}
