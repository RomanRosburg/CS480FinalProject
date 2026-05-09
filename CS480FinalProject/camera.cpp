#include "camera.h"

Camera::Camera()
{
    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    worldUp = cameraUp;
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 0.01f;
    mouseSensitivity = 0.1f;
    updateCamVectors();
}

Camera::~Camera()
{

}
// Called inside Graphics
bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  //view = glm::lookAt( glm::vec3(x, y, z), //Eye Position
  //                    glm::vec3(0.0, 0.0, 0.0), //Focus point
  //                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
  
  view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

  projection = glm::perspective( glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);;
}

void Camera::Translate(glm::vec3 translate, double dt)
{
    float velocity = speed * dt;
    cameraPos += translate * velocity;
}

void Camera::mouseLook(float mouseX, float mouseY) {
    mouseX *= mouseSensitivity;
    mouseY *= mouseSensitivity;

    yaw += mouseX;
    pitch += mouseY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCamVectors();
}

void Camera::lookAtPlanet()
{
}

void Camera::updateFollow(glm::mat4 model)
{
}

void Camera::updateCamVectors()
{
    // Calc new camera front vectors
    glm::vec3 newFront;
    newFront.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    newFront.y = glm::sin(glm::radians(pitch));
    newFront.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    cameraFront = glm::normalize(newFront);
    // Get right and up from front and world up
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    // calculate new view matrix
    //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}