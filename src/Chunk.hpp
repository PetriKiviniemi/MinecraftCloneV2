#ifndef CHUNK_H 
#define CHUNK_H 

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include "blocks/block.hpp"
#include <array>

static constexpr glm::vec3 CHUNK_SIZE = glm::vec3(6, 6, 6);
static constexpr int x_size = CHUNK_SIZE.x;
static constexpr int y_size = CHUNK_SIZE.y;
static constexpr int z_size = CHUNK_SIZE.z;

class Chunk
{
    private:
        glm::vec3 chunkPosition;
        void(Chunk::*m_callbackFuncPtr)();
    public:
        std::array<std::array<std::array<Block*, x_size>, y_size>,z_size> blocks;
        Chunk(glm::vec3 chunk_pos) { init(chunk_pos); };
        ~Chunk() {};

        glm::vec3 offset_from_origin;
        void init(glm::vec3 chunk_pos) { 
            offset_from_origin = chunk_pos;
            generate_chunk();
        }

        void generate_chunk();
};

#endif
