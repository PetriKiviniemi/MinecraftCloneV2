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

class Chunk;
static std::map<glm::vec3, Chunk*>::iterator get_iterator(glm::vec3 chunk_pos);
struct Vec3Comparator {
    inline bool operator() (glm::vec3 const &fobj, glm::vec3 const &sobj) const {
        return fobj.x < sobj.x && fobj.y < sobj.y && fobj.z < sobj.z;
    }
};

static constexpr glm::vec3 CHUNK_SIZE = glm::vec3(16, 16, 16);
static constexpr int x_size = CHUNK_SIZE.x;
static constexpr int y_size = CHUNK_SIZE.y;
static constexpr int z_size = CHUNK_SIZE.z;
static std::map<glm::vec3, Chunk*, Vec3Comparator> all_chunks;

class Chunk
{
    private:
        glm::vec3 chunkPosition;
        void(Chunk::*m_callbackFuncPtr)();
    public:
        std::array<std::array<std::array<Block*, x_size>, y_size>,z_size> blocks;
        std::map<std::string, Chunk*> neighbour_chunks;
        Chunk(glm::vec3 chunk_pos) {
            all_chunks.insert(
                std::pair<glm::vec3, Chunk*>(chunk_pos, this)
            );
            init(chunk_pos); 
        };
        ~Chunk() { 
            all_chunks.erase(get_iterator(offset_from_origin));
        };

        glm::vec3 offset_from_origin;
        void init(glm::vec3 chunk_pos) { 
            offset_from_origin = chunk_pos;
            generate_chunk();
        }

        void generate_chunk();
        void update_neighbour_chunks();
        std::vector<Block*> get_neighbour_blocks(Block* block);
};

static std::map<glm::vec3, Chunk*>::iterator get_iterator(glm::vec3 chunk_pos)
{
    return all_chunks.find(chunk_pos);
}

static Chunk* get_neighbour_chunk(glm::vec3 chunk_pos, glm::vec3 offset)
{
    std::map<glm::vec3, Chunk*>::iterator it;
    glm::vec3 pos = (glm::vec3(chunk_pos.x + offset.x, chunk_pos.y + offset.y, chunk_pos.z + offset.z));
    it = all_chunks.find(pos);
    if(it != all_chunks.end())
        return it->second;
    return nullptr;
};

#endif
