#ifndef TEXTURE_H
#define TEXTURE_H

#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

struct TextureAtlas {

    unsigned char* texture_atlas;
    int width, height, nrChannels;

    std::vector<std::vector<unsigned char>> textures;
    std::vector<unsigned int> texture_buffers;
    unsigned int one_texture_buffer;

    void load_texture_atlas()
    {
        texture_atlas = stbi_load(("..\\textures\\atlas.png"), &width, &height, &nrChannels, 0);
        int tile_size_x = 16 * 4;
        int row_len = 16 * tile_size_x;

        for(int i = 0; i < 16; i++)
        {

            for(int j = 0; j < 16; j++)
            {
                std::vector<unsigned char> t_tex;
                unsigned char* current_data_ptr = texture_atlas + i*row_len*16 + j*tile_size_x;

                for (int row = 0; row < 16; row++)
                {
                    std::copy(current_data_ptr + row*row_len, current_data_ptr + row*row_len + tile_size_x, std::back_inserter(t_tex));
                }

                textures.push_back(t_tex);
            }
        }
    }

    unsigned int get_texture_buffer(unsigned int texture_id)
    {
        return texture_buffers[texture_id];
    }

    unsigned int get_one_texture_buffer()
    {
        return one_texture_buffer;
    }

    void generate_textures() {

        glGenTextures(1, &one_texture_buffer);
        glBindTexture(GL_TEXTURE_2D, one_texture_buffer);

        //Configure texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_atlas);
        glGenerateMipmap(GL_TEXTURE_2D);

        for(int i = 0; i < textures.size(); i++)
        {
            unsigned char* texture_data = &textures[i][0];
            unsigned int texture_buff;

            glGenTextures(1, &texture_buff);
            glBindTexture(GL_TEXTURE_2D, texture_buff);

            //Configure texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
            glGenerateMipmap(GL_TEXTURE_2D);

            std::cout << "texture generated" << std::endl;
            texture_buffers.push_back(texture_buff);
            //stbi_image_free(Textudirt_data);
        }
    }

    TextureAtlas() {
        load_texture_atlas();
        generate_textures();
    }
};



#endif
