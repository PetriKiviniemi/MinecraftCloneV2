#ifndef RENDERER_H
#define RENDERER_H
#include <unordered_map>
#include <memory>
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include "blocks/block.hpp"
#include "chunk_renderer.hpp"

class Renderer
{
    private:
        static Renderer * _instance;
        ShaderProgram* sp;
    protected:
        Renderer();
    public:
        ~Renderer() {};
        Renderer(Renderer &other) = delete;
        void operator=(const Renderer &) = delete;
        static Renderer *getInstance();

        void init();
        void tick();

        float dTime = 0.0f;
        float lastFrame = 0.0f;
        float curFrame = 0.0f;
        int framerate = 0;

        std::shared_ptr<TextureAtlas> txAtlas;
        std::vector<ChunkRenderer*> chunk_renderers;

        //TODO:: Replace with view object that contains everything in view of the player!
        //void loadMesh(std::vector<float> data, unsigned int textureBuf);
        
        void clear_and_load_shaders(unsigned int wWidth, unsigned int wHeight);
        void add_chunk_renderer(ChunkRenderer* chunk_renderer) { chunk_renderers.push_back(chunk_renderer); };
        void render_chunks();
};


#endif
