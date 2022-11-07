#ifndef BLOCK_H
#define BLOCK_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <mutex>
#include "../src/texture.hpp"

/* TODO:: Consider making a graph of cubes. Every cube would have knowledge of its neighbours in 3D space
enum NeighbourBlockDir { 
    N = 1, S, W, E, NW, NE, SW, SE,
    FN, FS, FW, FE, FNW, FNE, FSW, FSE,
    BN, BS, BW, BE, BNW, BNE, BSW, BSE
};
*/

enum BlockFaceDir {
    N = 0, S, W, E, U, D 
};

struct Face;
static std::vector<float> generate_face_data(BlockFaceDir n);
static std::vector<float> generate_cube_vertex_data(std::map<BlockFaceDir, Face> faces);

static const glm::vec3 cube_vertices[] {
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 0, 1)
};

static const glm::vec2 texture_coords[] {
    glm::vec2(0, 0),
    glm::vec2(0, 1),
    glm::vec2(1, 1),
    glm::vec2(1, 0),
};

static const unsigned int texture_indices[] {
    2, 1, 0, 2, 0, 3,
};

// 0, 1, 2 right left
// 2, 3, 0 left
// 2, 0, 3 up
// 0, 3, 2 down left
// 3, 2, 1 right
// 1, 2, 3 up left

static const unsigned int cube_indices[] {
    3, 0, 1, 3, 1, 2, // (north (-z))
    4, 7, 6, 4, 6, 5, // (south (+z))
    0, 4, 5, 0, 5, 1, // (west  (-x))
    7, 3, 2, 7, 2, 6, // (east  (+x))
    2, 1, 5, 2, 5, 6, // (up    (+y))
    0, 3, 7, 0, 7, 4  // (down  (-y))
};


static const unsigned int unique_indices[] {
    6, 12, 18, 24, 30, 36
};


struct Face
{
    BlockFaceDir dir;
    unsigned int texture_buff;
    std::vector<float> data;
    Face(BlockFaceDir d, unsigned int tex_buff) {
        dir = d;
        texture_buff = tex_buff;
        data = generate_face_data(dir);
    }
    unsigned int get_texture_id() { return texture_buff; }
};

enum BlockType {
    GRASS,
    AIR
};

struct Block 
{
    glm::vec3 position;
    virtual std::map<BlockFaceDir, Face> get_faces() = 0;
    virtual BlockType get_block_type() = 0;

    explicit Block(glm::vec3 pos) { position = pos;};
    ~Block() {};
};

struct GrassBlock : public Block
{
    using Block::Block;
    BlockType b_type = BlockType::AIR;

    std::map<BlockFaceDir, Face> faces = std::map<BlockFaceDir, Face>{
        {BlockFaceDir::N, Face(BlockFaceDir::N, 3)},
        {BlockFaceDir::S, Face(BlockFaceDir::S, 3)},
        {BlockFaceDir::E, Face(BlockFaceDir::E, 3)},
        {BlockFaceDir::W, Face(BlockFaceDir::W, 3)},
        {BlockFaceDir::U, Face(BlockFaceDir::U, 0)},
        {BlockFaceDir::D, Face(BlockFaceDir::D, 2)},
    };

    std::map<BlockFaceDir, Face> get_faces() override { return faces; }
    BlockType get_block_type() override { return b_type; }
};


//TODO:: Utilize EBO's directly? Is it problematic with 2 triangles and textures for face 
static std::vector<float> generate_face_data(BlockFaceDir n)
{
    std::vector<float> data;

    int tex_idx = 0;
    for(int k = unique_indices[n]-6; k < unique_indices[n]; k++)
    {
        glm::vec3 tmp = cube_vertices[cube_indices[k]];
        data.push_back(tmp.x);
        data.push_back(tmp.y);
        data.push_back(tmp.z);

        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);

        if(tex_idx == 6)
            tex_idx = 0;
        glm::vec2 tex_coord = texture_coords[texture_indices[tex_idx]];

        data.push_back(tex_coord.x);
        data.push_back(tex_coord.y);
        tex_idx++;
    }

    return data;
}


//Function to mash and the mesh to one glob
//TODO:: Needs to implement texture globbing logic aswell
static std::vector<float> generate_cube_vertex_data(std::map<BlockFaceDir, Face> faces)
{
    std::vector<float> data = std::vector<float>();
    for(std::map<BlockFaceDir, Face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        data.insert(data.end(), it->second.data.begin(), it->second.data.end());
    }
    return data;
};


#endif
