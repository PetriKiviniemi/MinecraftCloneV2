#ifndef CUBE_H
#define CUBE_H

#define GLFW_INCLUDE_NONE
#include <STB/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <mutex>

/* TODO:: Consider making a graph of cubes. Every cube would have knowledge of its neighbours in 3D space
enum NeighbourCubeDir { 
    N = 1, S, W, E, NW, NE, SW, SE,
    FN, FS, FW, FE, FNW, FNE, FSW, FSE,
    BN, BS, BW, BE, BNW, BNE, BSW, BSE
};
*/

enum CubeFaceDir {
    N = 1, S, W, E, F, B
};

class Cube
{
    private:
        unsigned int texture;
        mutable std::mutex data_mutex;
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        //Cube position in chunk 
        unsigned int layerIdx, rowIdx, colIdx;
        unsigned int activeFacesCount = 6;

        std::vector<float> frontFace = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };
        std::vector<float> backFace = {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };
        std::vector<float> leftFace = {
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
        std::vector<float> rightFace = {
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
        std::vector<float> bottomFace = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        };
        std::vector<float> topFace = {
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };

        std::vector<float> frontFaceC, backFaceC, leftFaceC, rightFaceC, bottomFaceC, topFaceC;
        std::vector<float> data = {}; 
        std::vector<float> dataCopy = data;
    public:
        Cube();
        ~Cube() {};

        void initializeCube()
        {
            std::lock_guard<std::mutex> guard(data_mutex);
            std::vector<float>().swap(data);
            data.insert(data.end(), frontFaceC.begin(), frontFaceC.end());
            data.insert(data.end(), backFaceC.begin(), backFaceC.end());
            data.insert(data.end(), leftFaceC.begin(), leftFaceC.end());
            data.insert(data.end(), rightFaceC.begin(), rightFaceC.end());
            data.insert(data.end(), bottomFaceC.begin(), bottomFaceC.end());
            data.insert(data.end(), topFaceC.begin(), topFaceC.end());
        }

        const std::vector<float> getData() const {
            return data;
        }
        const glm::vec3 getPosition() const { return position; }
        const unsigned int getTextureBuffer() const { return texture; }
        const unsigned int getActiveFacesCount() const { return activeFacesCount; }

        void deleteFace(CubeFaceDir dir); 

        void restoreData() { data = dataCopy; }
        void setPosition(glm::vec3 pos) { position = pos; }
        void setColor(std::vector<float> color);
        void setTexture(std::string texture_path);

        void setLayerIdx(unsigned int idx) { layerIdx = idx; }
        void setRowIdx(unsigned int idx) { rowIdx = idx; }
        void setColIdx(unsigned int idx) { colIdx = idx; }
        void setChunkPos(unsigned int l, unsigned int r, unsigned int c) { layerIdx = l; rowIdx = r; colIdx = c; }

        void print_cube_state()
        {
            std::cout << " " << std::endl;
            std::cout << "Position in world: " << position[0] << ":" << position[1] << ":" << position[2] << std::endl;
            std::cout << "Position in chunk: " << layerIdx << ":" << rowIdx << ":" << colIdx << std::endl;
            std::cout << "Data: " << data.size() << std::endl;
            std::cout << "Active faces count: " << activeFacesCount << std::endl;
            std::cout << "Bound texture buffer id: " << texture << std::endl;
        }

};

#endif
