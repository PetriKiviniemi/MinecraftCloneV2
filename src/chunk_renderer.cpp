#define GLFW_INCLUDE_NONE
#include <array>
#include <glm/glm.hpp>
#include "chunk_renderer.hpp"
#include <stack>

void ChunkRenderer::bind_data_to_buffer(std::vector<float> data, unsigned int texture_buff)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, txAtlas->get_one_texture_buffer());

    //Unbind buffers, data is bound to vao now
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ChunkRenderer::render_face(Face* f, glm::vec3 position)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position); 
    sp->setMat4("model", model);

    //TODO:: Reduce bind calls to only occur when something has changed in the chunk
    //bind_face_data_to_buffers(f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ChunkRenderer::render_block(Block* block)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, block->position); 
    sp->setMat4("model", model);

    bind_data_to_buffer(block->data, txAtlas->one_texture_buffer);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ChunkRenderer::render_chunk(TextureAtlas* tx_atlas, ShaderProgram* shader_prog)
{
    txAtlas = tx_atlas;
    sp = shader_prog;
    for(const auto& block : blocks_to_render)
        render_block(block);
}

void ChunkRenderer::dfs(glm::vec3 pos, std::array<std::array<std::array<bool, x_size>,y_size>,z_size> *visited_nodes)
{
    std::stack<glm::vec3> st;
    st.push(pos);

    std::array<int, 6> dx = {1, -1, 0, 0, 0, 0};     
    std::array<int, 6> dy = {0, 0, 1, -1, 0, 0};     
    std::array<int, 6> dz = {0, 0, 0, 0, 1, -1}; 

    while(!st.empty())
    {
        glm::vec3 cur = st.top();
        st.pop();
        int x = cur.x;
        int y = cur.y;
        int z = cur.z;

        if(
            x < 0 || y < 0 || z < 0 ||
            x >= x_size || y >= y_size || z >= z_size
        )
            continue;

        if(visited_nodes->operator[](x)[y][z] == true)
            continue;

        (*visited_nodes)[x][y][z] = true;

        for(int i = 0; i < dx.size(); i++)
        {
            st.push(glm::vec3(x + dx[i], y + dy[i], z + dz[i]));
        }

    }
}

void ChunkRenderer::create_mesh()
{
    std::array<
        std::array<
            std::array<bool, x_size>,
            y_size>
        ,z_size
    >
    visited_nodes;

    Block* last_block;

    int x, y, z;
    for(const auto& i : chunk->blocks)
    {
        for(const auto& j : i)
        {
            for(const auto& block: j)
            {
                if(block->get_block_type() != BlockType::AIR && !visited_nodes[x][y][z])
                {
                    dfs(block->position, &visited_nodes);
                    last_block = block;
                }
                else if (last_block)
                {
                    blocks_to_render.push_back(last_block);
                    last_block = nullptr;
                }
                else if(block->get_block_type() != BlockType::AIR && Utils::is_chunk_boundary(block->position))
                {
                    blocks_to_render.push_back(last_block);
                }
                z++;

                //TODO:: Instead of checking boundary blocks,
                //check if all neighbouring cubes are not air -> do not render the block 
                //check neighbouring chunk cubes as well
                /*
                if(
                    (block->position.x + 1 < chunk->blocks.size() &&
                     chunk->blocks[block->position.x + 1][block->position.y][block->position.z]->get_block_type() != BlockType::AIR) &&
                    (block->position.x - 1 > 0 &&
                     chunk->blocks[block->position.x - 1][block->position.y][block->position.z]->get_block_type() != BlockType::AIR) &&

                    (block->position.z + 1 < chunk->blocks[0][0].size() &&
                     chunk->blocks[block->position.x][block->position.y][block->position.z + 1]->get_block_type() != BlockType::AIR) &&
                    (block->position.z - 1 > 0 &&
                     chunk->blocks[block->position.x][block->position.y][block->position.z - 1]->get_block_type() != BlockType::AIR) &&

                    (block->position.x + 1 < chunk->blocks.size() && block->position.z + 1 < chunk->blocks[0][0].size() &&
                     chunk->blocks[block->position.x + 1][block->position.y][block->position.z + 1]->get_block_type() != BlockType::AIR) &&
                    (block->position.x - 1 > 0 && block->position.z + 1 < chunk->blocks[0][0].size() &&
                     chunk->blocks[block->position.x - 1 ][block->position.y][block->position.z + 1]->get_block_type() != BlockType::AIR) &&

                    (block->position.x + 1 < chunk->blocks.size() && block->position.z - 1 > 0 &&
                     chunk->blocks[block->position.x + 1][block->position.y][block->position.z - 1]->get_block_type() != BlockType::AIR) &&
                    (block->position.x - 1 > 0 && block->position.z - 1 > 0 &&
                     chunk->blocks[block->position.x - 1][block->position.y][block->position.z - 1]->get_block_type() != BlockType::AIR)
                )
                {
                    blocks_to_render.push_back(block);
                }
                */
              //  if(
              //      block->position.x == chunk->offset_from_origin.x ||
              //      block->position.y == chunk->offset_from_origin.y ||
              //      block->position.z == chunk->offset_from_origin.z ||
              //      block->position.x == chunk->offset_from_origin.x + CHUNK_SIZE.x - 1||
              //      block->position.y == chunk->offset_from_origin.y + CHUNK_SIZE.y - 1||
              //      block->position.z == chunk->offset_from_origin.z + CHUNK_SIZE.z - 1
              //  )
              //  {
              //      blocks_to_render.push_back(block);
              //  }
            }
            y++;
        }
        x++;
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
