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
#include "Cube.hpp"
#include <array>

constexpr unsigned int CHUNK_MAX_LAYERS = 8;
constexpr unsigned int CHUNK_MAX_ROWS = 6;
constexpr unsigned int CHUNK_MAX_COLS = 6;

class Chunk
{
    private:
        std::array<std::array<std::array<std::shared_ptr<Cube>, CHUNK_MAX_COLS>,CHUNK_MAX_ROWS>,CHUNK_MAX_LAYERS> cubes{nullptr};
        glm::vec3 chunkPosition;
        unsigned int currentLayerCount = CHUNK_MAX_LAYERS; //Variable used for testing the chunk's size
        void(Chunk::*m_callbackFuncPtr)();
    public:
        Chunk(glm::vec3 chunkPos);
        ~Chunk() {};

        //TODO::
        //1. Implement chunk position in world (offset from origin)

        void initializeChunk(glm::vec3 chunkPos);
        void createLayer(int layerPos, glm::vec3 chunkPos);
        std::thread createLayerAsync(int layerPos, glm::vec3 chunkPos);

        std::array<std::array<std::array<std::shared_ptr<Cube>, CHUNK_MAX_COLS>,CHUNK_MAX_ROWS>,CHUNK_MAX_LAYERS> getCubes() { return cubes; }

        const glm::vec3 getChunkPos() const { return chunkPosition; }
        void setChunkPos(glm::vec3 pos) { chunkPosition = pos; }

        void replaceCubeAtIndex(std::shared_ptr<Cube> c, unsigned int idx) { /* TODO:: IMPLEMENT */ }
        Cube* getCubeAtIndex(unsigned int lIdx, unsigned int rIdx, unsigned int cIdx) {return cubes[lIdx][rIdx][cIdx].get();}

        void generateTextures(
            std::shared_ptr<Cube> c
        );

        void removeDuplicateFaces(
           std::shared_ptr<Cube> c,
            unsigned int lIdx,
            unsigned int rIdx,
            unsigned int cIdx,
            unsigned int layerCount,
            unsigned int rowCount,
            unsigned int colCount 
        );

        static void removeDuplicateFaces_static(
            void* arg,
            std::shared_ptr<Cube> c,
            unsigned int lIdx,
            unsigned int rIdx,
            unsigned int cIdx,
            unsigned int layerCount,
            unsigned int rowCount,
            unsigned int colCount 
        )
        {
            Chunk* that = (Chunk*)arg;
            std::thread t1(&Chunk::removeDuplicateFaces, that, c,lIdx,rIdx,cIdx,layerCount,rowCount,colCount);
            t1.detach();
        }

        void print_chunk_info()
        {
            for(int i = 0; i < cubes.size(); i++)
            {
                for(int j = 0; j < cubes.at(0).size(); j++)
                {
                    for(int k = 0; k < cubes.at(0).at(0).size(); k++)
                    {
                        cubes.at(i).at(j).at(k).get()->print_cube_state();
                    }
                }
            }
        }

        /* NOTE:: C++ COMPILER PROBABLY CANNOT INLINE THIS */
        /* FOR STATIC FUNCTIONS ONLY */
        
        /* Pass any function that takes following parameters:
         * std::shared_ptr<Cube> cube
         * Int layerIdx
         * Int rowIdx
         * Int colIdx
         * */
        template <typename Func>
        void funcCallbackIterateCubes(Func f)
        {
            int layerC = 0;
            std::array<std::array<std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS>,CHUNK_MAX_ROWS>,CHUNK_MAX_LAYERS>::iterator layers;
            for(layers = cubes.begin(); layers != cubes.end(); layers++)
            {
                int rowC = 0;
                std::array<std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS>,CHUNK_MAX_ROWS>::iterator rows;
                for(rows = layers->begin(); rows != layers->end(); rows++)
                {
                    int colC = 0;
                    std::array<std::shared_ptr<Cube>,CHUNK_MAX_COLS>::iterator cols;
                    for(cols = rows->begin(); cols != rows->end(); cols++)
                    {

                        f(this, (*cols),layerC, rowC, colC, cubes.size(), cubes.at(0).size(), cubes.at(0).at(0).size());
                        colC++;
                    }
                    rowC++;
                }
                layerC++;
            }
        }

};

#endif
