#include <stdlib.h>
#include <utility>
#include "Chunk.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include <iostream>
#include <fstream>

World::World()
{
    load();
}

void World::load()
{
    int chunk_c = 20;
    std::vector<std::thread> chunk_threads;

    for(int i = 0; i < 1; i++)
    {
        for(int j = 0; j < 1; j++)
        {
            glm::vec3 newChunkPos = glm::vec3(0.0f, 0.0f, 0.0f);
            newChunkPos[0] += static_cast<int>(j * static_cast<int>(CHUNK_MAX_ROWS));
            newChunkPos[2] += static_cast<int>(i * static_cast<int>(CHUNK_MAX_COLS));

            addLoadedChunk(Utils::generateHashForVec3AsInts(newChunkPos), std::shared_ptr<Chunk>(new Chunk(newChunkPos)));
            //chunk_threads.push_back(generateNewChunkAsync(newChunkPos));
            std::cout << "Loading: " << (i*10)+j << "%" << std::endl;
        }
    }

    loadedChunks[0].get()->print_chunk_info();

    std::vector<std::thread>::iterator threads_it;
    for(threads_it = chunk_threads.begin(); threads_it != chunk_threads.end(); threads_it++)
    {
        (*threads_it).join();
    }
}

std::thread World::generateNewChunkAsync(glm::vec3 chunkPos)
{
    return std::thread([=] {
            addLoadedChunk(Utils::generateHashForVec3AsInts(chunkPos), std::shared_ptr<Chunk>(new Chunk(chunkPos)));
            });
}

void World::loadChunks()
{
    /* TODO::
     * New logic
     * Instead of modifying the large chunk array at runtime (everyframe)
     * Have a pointer of chunks in Renderer
     * Lock this memory space with a mutex
     * Check every frame whether player crossed boundaries to new chunk 
     * If that's the case, lock the mutex here, and load the new chunks from preloaded world
     * */

    Camera* cam = Camera::getInstance();
    glm::vec3 camPos = cam->getPosition();


    std::vector<float> hashList = {};
    //Check 5x5 area of chunks around the player


    for(int i = -2; i < 2; i++)
    {
        for(int j = -2; j < 2; j++)
        {
            glm::vec3 pos = glm::vec3(0,0,0);
            int nearestX = Utils::roundToNearestMultipleOf(int(camPos[0]), CHUNK_MAX_ROWS);
            int nearestY = Utils::roundToNearestMultipleOf(int(camPos[2]), CHUNK_MAX_COLS);
            pos[0] = nearestX + (j  * static_cast<int>(CHUNK_MAX_ROWS));
            pos[2] = nearestY + (i  * static_cast<int>(CHUNK_MAX_COLS));
            //Hash the positions next to player
            //Add these hashes to a list
            float hashCode = Utils::generateHashForVec3AsInts(pos);
            hashList.push_back(hashCode);
        }
    }
}

void World::prepare_frame()
{
    //Check if something has changed in the loaded chunks (Size)
}
