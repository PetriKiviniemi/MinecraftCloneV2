#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/exponential.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

Camera* Camera::_instance{nullptr}; 
std::mutex Camera::_cam_mutex;

Camera::Camera()
{
    init();
}

Camera* Camera::getInstance()
{
    std::lock_guard<std::mutex> lock(_cam_mutex);
    if(_instance == nullptr)
    {
        _instance = new Camera();
    }
    return _instance;
}

void Camera::init()
{
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //Initialize vectors for glm::lookAt matrix
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = -89.0f;
    roll = 0.0f;        //Not necessary for ingame camera. Maybe for cinematic purposes
}

void Camera::camLookAt()
{
    view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget, cameraUp);
}


void Camera::turnLeft(float speed)
{
    //Define an angle to use for quaternion rotation
    //Calculate necessary vectors
    glm::vec3 camDir = glm::normalize(cameraPosition - cameraTarget);
    glm::vec3 camXaxis = glm::normalize(glm::cross(cameraUp, camDir));
    glm::vec3 camNewUp = glm::cross(camXaxis, camDir);
    
    float angle = 1.0f;
    //NOTE:: Quaternion way of doing this. Euler angles seem to be better for 3D camera.
    //Quaternion for ingame objects?
    /**
     * Quaternions:
     * a = angle to rotate
     * vU = axis to rotate around as unit vector
     * Q = [cos(a/2), sin(a/2)*vU.x, sin(a/2)*vU.y, sin(a/2)*vU.z]
     
    glm::vec4 q = glm::cos(angle/2) + glm::normalize(cameraUp)*glm::sin(angle/2);
    */ 
}

void Camera::turnRight(float speed)
{
}

void Camera::lookUp(float speed)
{
}

void Camera::lookDown(float speed)
{
}

void Camera::rotate(float mXoffset, float xYoffset)
{
    yaw += mXoffset;
    pitch += xYoffset;

    if(pitch > 89.0f)
       pitch = 89.0f;
    if(pitch < -89.0f)
       pitch = -89.0f; 

    //Calculate new camera direction
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraTarget = glm::normalize(dir);
    camLookAt();
}

void Camera::moveLeft(float speed)
{
    cameraPosition -= speed * glm::normalize(glm::cross(cameraTarget, cameraUp));
    camLookAt();
}

void Camera::moveRight(float speed)
{
    cameraPosition += speed * glm::normalize(glm::cross(cameraTarget, cameraUp));
    camLookAt();
}

void Camera::forward(float speed)
{
    cameraPosition += speed * cameraTarget;
    camLookAt();
}

void Camera::reverse(float speed)
{
    cameraPosition -= speed * cameraTarget;
    camLookAt();
}

bool Camera::checkForCollision(Block* c)
{
    return true;
}
