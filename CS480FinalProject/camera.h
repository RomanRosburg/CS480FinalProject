#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "mesh.h"

class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    /// Camera movement. TODO: add zoom, and aim to planet
    // Assums a vec3 with no value greater than 1, speed obtained inside translate
    void Translate(glm::vec3 translate, double dt);
    void mouseLook(float mouseX, float mouseY);
    void lookAtPlanet();
    void updateFollow(glm::mat4* model); // Get the Model Matrix of the Ship to follow?
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }
    double getPitch() { return pitch; }
    double getYaw() { return yaw; }
    float getSpeed() { return speed; }
    void setX(double x); // For mouse looking
    void setY(double y); // For mouse looking
    void setZ(double z); // For zoom
    void setPos(glm::vec3 pos);
    glm::mat4* getTarget() { return target; }
    glm::vec3 getPos() { return cameraPos; }
    glm::vec3 getFront() { return cameraFront; }
    glm::vec3 getUp() { return cameraUp; }
    glm::vec3 getRight() { return cameraRight; }
  
  private:

    double x = 0.0;
    double y = 0.0;
    double z = 3.0;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4* target;

    // Camera movement vars given kindly from learnopengl
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    float speed;
    float mouseSensitivity;
    float yaw;
    float pitch;

    void updateCamVectors();
};

#endif /* CAMERA_H */
