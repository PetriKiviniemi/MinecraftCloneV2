#define GLFW_INCLUDE_NONE
#include <glm/glm.hpp>
#include "chunk_renderer.hpp"

void ChunkRenderer::bind_face_data_to_buffers(Face* f)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, f->data.size() * sizeof(float), &f->data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::cout << "Binding face data" << std::endl;
    //Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, txAtlas->get_texture_buffer(f->texture_buff));

    //Unbind buffers, data is bound to vao now
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ChunkRenderer::render()
{
    /* CREATE CHUNK MESH */
}

void ChunkRenderer::render_face(Face* f, glm::vec3 position)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); 
    sp->setMat4("model", model);

    //TODO:: Reduce bind calls to only occur when something has changed in the chunk
    bind_face_data_to_buffers(f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ChunkRenderer::render_block(Block* block)
{
    std::map<BlockFaceDir, Face> faces = block->get_faces();
    for(std::map<BlockFaceDir, Face>::iterator it = faces.begin(); it != faces.end(); it++)
    {
        Face* face = &it->second;
        std::cout << "iterating_faces" << std::endl;
        render_face(face, block->position);
    }
}

void ChunkRenderer::render_chunk(TextureAtlas* tx_atlas, ShaderProgram* shader_prog)
{
    txAtlas = tx_atlas;
    sp = shader_prog;
    for(const auto& i : chunk->blocks)
    {
        for(const auto& j : i)
        {
            for(const auto& block : j)
            {
                std::cout << "rendering_chunk" << std::endl;
                render_block(block);
            }
        }
    }
}

void ChunkRenderer::tick()
{
    //Check if anything has changed
    if(has_changed)
    {
        has_changed = false;
    }
}
