#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <vector>
#include "TextureManager.hpp"
#include "Cube.hpp"
#include <iostream>
#include <vector>

Cube::Cube()
{
    frontFaceC = frontFace;
    backFaceC = backFace;
    leftFaceC = leftFace;
    rightFaceC = rightFace;
    bottomFaceC = bottomFace;
    topFaceC = topFace;
    initializeCube();
}

void Cube::setTexture(std::string texture_path)
{
    std::lock_guard<std::mutex> guard(data_mutex);
    texture = TextureManager::getInstance()->createTexture(texture_path);
}

/* TODO:: 
 * Is there a way to optimize this without 
 * having to iterator every face's vertex data with the deletion of single face?
 * */
void Cube::deleteFace(CubeFaceDir dir)
{
    switch(dir)
    {
        case N:
            // NOTE:: Clear does not free the memory but only destroys the objects.
            // Allocating memory again could be more performance consuming?
            if(topFaceC.size() > 0)
            {
                topFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
        case S:
            if(bottomFaceC.size() > 0)
            {
                bottomFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
        case W:
            if(leftFaceC.size() > 0)
            {
                leftFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
        case E:
            if(rightFaceC.size() > 0)
            {
                rightFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
        case F:
            if(frontFaceC.size() > 0)
            {
                frontFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
        case B:
            if(backFaceC.size() > 0)
            {
                backFaceC.clear();
                activeFacesCount -= 1;
            }
            break;
            
    }

    initializeCube();
}
