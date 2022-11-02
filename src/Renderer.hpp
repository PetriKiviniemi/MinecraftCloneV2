#ifndef RENDERER_H
#define RENDERER_H
#include <unordered_map>
#include <memory>
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include "Chunk.hpp"

class Renderer
{
    private:
        static Renderer * _instance;
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        ShaderProgram* sp;
    protected:
        Renderer();
    public:
        ~Renderer() {};
        Renderer(Renderer &other) = delete;
        void operator=(const Renderer &) = delete;
        static Renderer *getInstance();

        //Framerate
        float dTime = 0.0f;
        float lastFrame = 0.0f;
        float curFrame = 0.0f;
        int framerate = 0;

        void init();
        void updateScreenDimensions(unsigned int w, unsigned int h);
        void prepare_frame();

        std::unordered_map<glm::vec3, std::unique_ptr<Chunk>> chunks_to_render;
        std::unordered_map<unsigned int, unsigned int> vbo_buffers;

        void add_chunk(Chunk* c, glm::vec3 hash) {
            chunks_to_render.insert(std::make_pair(hash, std::unique_ptr<Chunk>(c)));
        }


        inline Chunk* get_chunk(glm::vec3 hash) {
            return chunks_to_render.contains(hash) ?
                chunks_to_render[hash].get() :
                nullptr;
        }
        void addData(std::vector<float> data, unsigned int textureBuf);
        void clear_and_load_shaders(unsigned int wWidth, unsigned int wHeight);

        void drawCube(std::shared_ptr<Cube> c, unsigned int wWidth, unsigned int wHeight);
        void drawChunk(std::shared_ptr<Chunk> c, unsigned int wWidth, unsigned int wHeight);
        void draw_chunks(
                const std::unordered_map<float, std::shared_ptr<Chunk>>& chunkMap,
                unsigned int wWidth,
                unsigned int wHeight
        );
};


#endif
