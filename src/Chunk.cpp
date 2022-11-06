#include "Chunk.hpp"
#include <vector>
#include <memory>
#include <array>
#include <iostream>
#include <thread>
#include <chrono>

void Chunk::generate_chunk()
{
    for(int i = 0; i < x_size; i++)
    {
        for(int j = 0; j < y_size; j++)
        {
            for(int k = 0; k < z_size; k++)
            {
                blocks[i][j][k] = new GrassBlock(glm::vec3(i,j,k));
            }
        }
    }
}
