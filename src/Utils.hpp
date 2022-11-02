#ifndef UTILS_H
#define UTILS_H

#define GLFW_INCLUDE_NONE
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Chunk.hpp"

namespace Utils
{
    static float cantorPairingFunc(int a, int b)
    {
        return 0.5*(a + b)*(a + b + 1) + b;
    }

    //TODO:: REPLACE WITH OFFSET FROM ORIGIN (0,0,0), chunks grow from bottom left to 2d space
    static float generateHashForVec3AsInts(glm::vec3 pos)
    {
        int x = static_cast<int>(pos[0]);
        int y = static_cast<int>(pos[1]);
        int z = static_cast<int>(pos[2]);

        //Generate hash using the specific order of the cells
        //Cantor pairing function
        return 0.5*(cantorPairingFunc(x,y) + z)*(cantorPairingFunc(x,y) + z + 1) + z;  
    }

    static bool isPlayerInChunk(glm::vec3 playerPos, glm::vec3 chunkPos)
    {
        int px = static_cast<int>(playerPos[0]);
        int py = static_cast<int>(playerPos[1]);
        int pz = static_cast<int>(playerPos[2]);

        int cx = static_cast<int>(chunkPos[0]);
        int cy = static_cast<int>(chunkPos[1]);
        int cz = static_cast<int>(chunkPos[2]);

        //Only px and pz relevenant to check
        if(px > cx && pz > cz)
        {
            if(
                px <= cx + CHUNK_MAX_ROWS && px >= cx &&
                pz <= cz + CHUNK_MAX_COLS && pz >= cz
            )
            {
                //Player in chunk
            std::cout << "Player in chunk: " << generateHashForVec3AsInts(chunkPos) << std::endl;
            return true;
            }
        }
        return false;
    }

    static bool getPlayerCurrentChunkPos(glm::vec3 playerPos)
    {
    }

    static std::string vec3toString(glm::vec3 pos)
    {
       return std::to_string(pos[0]) + ":" + std::to_string(pos[1]) + ":" + std::to_string(pos[2]);
    }

    //TODO:: FIX
    static int roundToNearestMultipleOf(int original, unsigned int multipleOf)
    {
        if(multipleOf == 0 || (original % multipleOf) == 0)
            return original;

        //If we're dealing with negative integers, just round up
        return original + (multipleOf - (original % multipleOf));
    }

}

#endif
