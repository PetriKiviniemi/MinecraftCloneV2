#include <string>
#include <memory>
#include <mutex>

/*This class's purpose is to hold different textures in memory and distribute them to cubes*/
class TextureManager
{
    private:
        static TextureManager * _instance;
        static std::mutex _mutex;
    protected:
        TextureManager();
    public:
        ~TextureManager() {};
        TextureManager(TextureManager &other) = delete;
        void operator=(const TextureManager &) = delete;
        static TextureManager *getInstance();

        unsigned int createTexture(std::string texture_path);
};
