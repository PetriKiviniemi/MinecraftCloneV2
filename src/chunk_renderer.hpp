#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Utils.hpp"
#include "Chunk.hpp"
#include "blocks/block.hpp"
#include "ShaderProgram.hpp"

class ChunkRenderer : Utils::Tickable
{
    public:
        Chunk* chunk;
        unsigned int vbo;
        unsigned int vao;
        ShaderProgram* sp;
        TextureAtlas* txAtlas;
        bool has_changed = true;
        
        ChunkRenderer(Chunk* c) { 
            chunk = c;
            glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao); 
        };
        ~ChunkRenderer() {};
        
        void tick();
        void bind_face_data_to_buffers(Face* f);
        void render_face(Face* f, glm::vec3 position);
        void render_block(Block* block);
        void render_chunk(TextureAtlas* txAtlas, ShaderProgram* sp);
        void render();
};
#endif
