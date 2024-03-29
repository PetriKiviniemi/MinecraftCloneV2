#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "Camera.hpp"
#include "Chunk.hpp"

class World
{
    private:
        //Float is the hash key generated by Utils::generateHash...
        std::unordered_map<float, std::shared_ptr<Chunk>> loadedChunks;
        glm::vec3 renderDistance = glm::vec3(10.0f, 0.0f, 10.0f);
        void load();
    public:
        World();
        ~World() {}

        std::unordered_map<float, std::shared_ptr<Chunk>> getLoadedChunks() { return loadedChunks; }
        void loadChunks();
        std::thread generateNewChunkAsync(glm::vec3 chunkPos);
        void addLoadedChunk(float key, std::shared_ptr<Chunk> c) { loadedChunks[key] = std::move(c); }
        void prepare_frame();

};

#endif
