#ifndef RENDERER_H
#define RENDERER_H
#include <unordered_map>
#include <memory>
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include "blocks/Cube.hpp"

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

        //TODO:: Replace with view object that contains everything in view of the player!
        //void loadMesh(std::vector<float> data, unsigned int textureBuf);

        void temp_render_cube(Cube* cube);
        void temp_add_data(std::vector<float> data, unsigned int textureBuf);

        void clear_and_load_shaders(unsigned int wWidth, unsigned int wHeight);
};


#endif
