#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

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
    void updateFollow(glm::mat4 model); // Get the Model Matrix of the Ship to follow?
  
  private:

    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;


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
