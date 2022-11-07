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
                blocks[i][j][k] = new GrassBlock(
                    glm::vec3(
                        offset_from_origin.x + i,
                        offset_from_origin.y + j,
                        offset_from_origin.z + k
                ));
            }
        }
    }
}

void Chunk::update_neighbour_chunks()
{
    glm::vec3 N = glm::vec3(0, 0, 16);
    glm::vec3 S = glm::vec3(0, 0, -16);
    glm::vec3 W = glm::vec3(-16, 0, 0);
    glm::vec3 E = glm::vec3(16, 0, 0);
    glm::vec3 NW = glm::vec3(-16, 0, 16);
    glm::vec3 NE = glm::vec3(16, 0, 16);
    glm::vec3 SW = glm::vec3(-16, 0, -16);
    glm::vec3 SE = glm::vec3(16, 0, -16);
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("N", get_neighbour_chunk(offset_from_origin, N)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("S", get_neighbour_chunk(offset_from_origin, S)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("W", get_neighbour_chunk(offset_from_origin, W)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("E", get_neighbour_chunk(offset_from_origin, E)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("NW", get_neighbour_chunk(offset_from_origin, NW)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("NE", get_neighbour_chunk(offset_from_origin, NE)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("SW", get_neighbour_chunk(offset_from_origin, SW)));
    neighbour_chunks.insert(std::pair<std::string, Chunk*>("SE", get_neighbour_chunk(offset_from_origin, SE)));
}
