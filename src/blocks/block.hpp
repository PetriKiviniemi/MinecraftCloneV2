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
static std::vector<float> generate_face_data(Face* f);
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
    glm::vec2(0, 0.0625),
    glm::vec2(0.0625, 0.0625),
    glm::vec2(0.0625, 0),
};

static const unsigned int texture_indices[] {
    2, 1, 0, 2, 0, 3,
};

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
    glm::vec2 texture_offset;
    std::vector<float> data;
    Face(BlockFaceDir d, glm::vec2 tex_offset) {
        dir = d;
        texture_offset = tex_offset;
        data = generate_face_data(this);
    }
    glm::vec2 get_texture_id() { return texture_offset; }
};

enum BlockType {
    GRASS,
    AIR
};

struct Block 
{
    glm::vec3 position;
    std::vector<float> data;
    virtual std::map<BlockFaceDir, Face*> get_faces() = 0;
    virtual BlockType get_block_type() = 0;
    virtual std::vector<float> get_data() = 0;

    std::map<BlockFaceDir, Face*> faces = std::map<BlockFaceDir, Face*>();

    void generate_mesh() {
        data.insert(data.end(), faces[BlockFaceDir::N]->data.begin(), faces[BlockFaceDir::N]->data.end());
        data.insert(data.end(), faces[BlockFaceDir::S]->data.begin(), faces[BlockFaceDir::S]->data.end());
        data.insert(data.end(), faces[BlockFaceDir::W]->data.begin(), faces[BlockFaceDir::W]->data.end());
        data.insert(data.end(), faces[BlockFaceDir::E]->data.begin(), faces[BlockFaceDir::E]->data.end());
        data.insert(data.end(), faces[BlockFaceDir::U]->data.begin(), faces[BlockFaceDir::U]->data.end());
        data.insert(data.end(), faces[BlockFaceDir::D]->data.begin(), faces[BlockFaceDir::D]->data.end());
    }

    explicit Block(glm::vec3 pos) { position = pos; };
    ~Block() {};
};

struct GrassBlock : public Block
{
    using Block::Block;
    BlockType b_type = BlockType::AIR;

    void set_grass_data() {
        faces = std::map<BlockFaceDir, Face*>();
        faces[BlockFaceDir::N] = new Face(BlockFaceDir::N, glm::vec2(3, 0));
        faces[BlockFaceDir::S] = new Face(BlockFaceDir::S, glm::vec2(3, 0));
        faces[BlockFaceDir::E] = new Face(BlockFaceDir::E, glm::vec2(3, 0));
        faces[BlockFaceDir::W] = new Face(BlockFaceDir::W, glm::vec2(3, 0));
        faces[BlockFaceDir::U] = new Face(BlockFaceDir::U, glm::vec2(0, 0));
        faces[BlockFaceDir::D] = new Face(BlockFaceDir::D, glm::vec2(2, 0));
    }

    std::map<BlockFaceDir, Face*> get_faces() override { return faces; }
    BlockType get_block_type() override { return b_type; }
    std::vector<float> get_data() override { return data; }

    explicit GrassBlock(glm::vec3 pos) : Block(pos) {position = pos; set_grass_data(); generate_mesh(); };
};


//TODO:: Utilize EBO's directly? Is it problematic with 2 triangles and textures for face 
static std::vector<float> generate_face_data(Face* face)
{
    std::vector<float> data;

    int tex_idx = 0;
    for(int k = unique_indices[face->dir]-6; k < unique_indices[face->dir]; k++)
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

        data.push_back(tex_coord.x + face->texture_offset.x * 0.0625);
        data.push_back(tex_coord.y + face->texture_offset.y * 0.0625);
        tex_idx++;
    }

    return data;
}

#endif
