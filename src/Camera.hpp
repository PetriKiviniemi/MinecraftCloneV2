#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Cube.hpp"
#include <vector>
#include <mutex>

class Camera
{
    private:
        static std::mutex _cam_mutex;
        static Camera* _instance;
        glm::mat4 view;
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraUp;
        float yaw, pitch, roll;
        Camera();
    public:
        ~Camera();
        const glm::mat4 getView() const { return view; }
        const glm::vec3 getPosition() 
        {
            std::lock_guard<std::mutex> guard(_cam_mutex);
            return cameraPosition;
        }
        static Camera* getInstance();
        void setCamPos(glm::vec3 camPos) { cameraPosition = camPos;}
        void init();
        void camLookAt();
        void turnLeft(float speed);
        void turnRight(float speed);
        void lookUp(float speed);
        void lookDown(float speed);

        void forward(float speed);
        void reverse(float speed);
        void moveLeft(float speed);
        void moveRight(float speed);

        void rotate(float mXoffset, float mYoffset);
        bool checkForCollision(Cube* c);
};

#endif
