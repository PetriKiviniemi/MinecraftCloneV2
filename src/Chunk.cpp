#include "Chunk.hpp"
#include <vector>
#include <memory>
#include <array>
#include <iostream>
#include <thread>
#include <chrono>


Chunk::Chunk(glm::vec3 chunkPos)
{
    initializeChunk(chunkPos);
}

void Chunk::initializeChunk(glm::vec3 chunkPos)
{
    chunkPosition = chunkPos;

    std::array<std::array<std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS>,CHUNK_MAX_ROWS>,CHUNK_MAX_LAYERS>::iterator cubesIt;

    int layer_amount = cubes.size();
    std::vector<std::thread> layer_threads;
    int layerC = 0;
    for(cubesIt = cubes.begin(); cubesIt != cubes.end(); cubesIt++)
    {
        //Multithreaded layer initialization
        layer_threads.push_back(createLayerAsync(layerC, chunkPos));
        //createLayer(layerC, chunkPos);
        layerC++;
    }

    std::vector<std::thread>::iterator threads_it;
    for(threads_it = layer_threads.begin(); threads_it != layer_threads.end(); threads_it++)
    {
        (*threads_it).join();
    }

    //After the chunk data is created, create textures in main thread
    //TODO:: Is there a better way to do this? Textures have to be created in the thread that
    //owns OpenGl context
    
    for(int i = 0; i < cubes.size(); i++)
    {
        for(int j = 0; j < cubes.at(0).size(); j++)
        {
            for(int k = 0; k < cubes.at(0).at(0).size(); k++)
            {
                generateTextures(cubes[i][j][k]);
            }
        }
    }
}

std::thread Chunk::createLayerAsync(int layerPos, glm::vec3 chunkPos)
{
    return std::thread([=] {createLayer(layerPos, chunkPos);});
}

void Chunk::createLayer(int layerPos, glm::vec3 chunkPos)
{
    std::array<std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS>,CHUNK_MAX_ROWS> rows;

    for(int i = 0; i < rows.size(); i++)
    {
        std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS> cols;

        for(int j = 0; j < cols.size(); j++)
        {
            std::shared_ptr<Cube> cube(new Cube());
            cube->setPosition(
                glm::vec3(chunkPos[0] + 1*i, chunkPos[1] + 1*layerPos, chunkPos[2] + 1*j)
            );
            cube->setChunkPos(layerPos, i, j);
            cols[j] = std::move(cube);
        }
        rows[i] = cols;
    }
    cubes[layerPos] = rows;
}

void Chunk::generateTextures(
    std::shared_ptr<Cube> c
)
{
    c->setTexture("Dirt.png");
}

void Chunk::removeDuplicateFaces(
        std::shared_ptr<Cube> c,
        unsigned int lIdx,
        unsigned int rIdx,
        unsigned int cIdx,
        unsigned int layerCount,
        unsigned int rowCount,
        unsigned int colCount 
)
{
    //Check neighbouring cubes. If they're not nullptr, remove duplicate vertices
    if(lIdx + 1 < layerCount-1)
    {
        Cube* neighbour = getCubeAtIndex(lIdx + 1, rIdx, cIdx);
        if(neighbour)
        {
            c->deleteFace(N);
            neighbour->deleteFace(S);
        }
    }
   if(lIdx != 0 && lIdx - 1 >= 0)
   {
       Cube* neighbour = getCubeAtIndex(lIdx - 1, rIdx, cIdx);
       if(neighbour)
       {
           c->deleteFace(S);
           neighbour->deleteFace(N);
       }
   } 
   
   if(rIdx + 1 < rowCount-1)
   {
       Cube* neighbour = getCubeAtIndex(lIdx, rIdx + 1, cIdx);
       if(neighbour)
       {
           c->deleteFace(E);
           neighbour->deleteFace(W);
       }
   }
   if(rIdx != 0 && rIdx - 1 >= 0)
   {
       Cube* neighbour = getCubeAtIndex(lIdx, rIdx - 1, cIdx);
       if(neighbour)
       {
           c->deleteFace(W);
           neighbour->deleteFace(E);
       }
   }
    
   if(cIdx + 1 < colCount-1)
   {
       Cube* neighbour = getCubeAtIndex(lIdx, rIdx, cIdx + 1);
       if(neighbour)
       {
           c->deleteFace(B);
           neighbour->deleteFace(F);
       }
   }
   if(cIdx != 0 && cIdx - 1 >= 0)
   {
       Cube* neighbour = getCubeAtIndex(lIdx, rIdx, cIdx - 1);
       if(neighbour)
       {
           c->deleteFace(F);
           neighbour->deleteFace(B);
       }
   }
}
