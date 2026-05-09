#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    // 3 Seperate systems for the different planets, their orbits, and the bodies orbiting the planet, all revolving around the sun
    void HierarchicalUpdateSystem1(double dt);
    void HierarchicalUpdateSystem2(double dt);
    void HierarchicalUpdateSystem3(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    // Main Sun
    Sphere* m_sun;

    // System 1
    Sphere* m_mars;

    // System 2
    Sphere* m_Jupiter;

    // Player Ship
    Mesh* m_mesh;

    // Asteroid Belt
    Sphere* m_ceres;

    Mesh* m_skybox;

};

#endif /* GRAPHICS_H */
