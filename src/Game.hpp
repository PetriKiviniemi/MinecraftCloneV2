#include <vector>
#include "Cube.hpp"
#include "Chunk.hpp"
#include "World.hpp"
#include "Renderer.hpp"


class Game
{
    private:
        static Game* _instance;
        void init(GLFWwindow* window);
        GLFWwindow* window;
    protected:
        Game(GLFWwindow* window) {init(window);};
    public:
        ~Game() {
            delete renderer;
            delete camera;
            delete world;
        };
        Game(Game &other) = delete;
        void operator=(const Game &) = delete;
        static Game* getInstance(GLFWwindow* window);

        void processInput();

        void prepare_frame();
        void update();
        void draw_frame();

        Renderer* renderer;
        Camera* camera;
        World* world;
};
