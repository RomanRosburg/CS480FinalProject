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
#include "Light.h"

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
    void moveShip(glm::vec3 moveDir, double dt);
    void Render();

    Camera* getCamera() { return m_camera; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    bool materialSetup();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera* m_camera;
    Shader* m_shader;
    Light*  m_light;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_normalMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;
    GLint m_hasN;

    /// New shader variables
    GLuint globalAmbLoc;
    GLuint lightALoc;
    GLuint lightDLoc;
    GLuint lightSLoc;
    GLuint lightPosLoc;

    /// Planets
    Sphere* m_venus;
    Sphere* m_mercury;
    // Saturn + ring
    Sphere* m_saturn;
    Mesh* m_saturnRing; // Potentially create a circle mesh for saturn?
    Sphere* m_mars;
    Sphere* m_jupiter;
    // Earth + moon
    Sphere* m_earth;
    Sphere* m_moon;

    Sphere* m_neptune;
    Sphere* m_uranus;

    /// Sun
    Sphere* m_sun;

    /// Asteroids/Other bodies(dwarf planets, comets)
    Sphere* m_ceres;
    Sphere* m_eris;
    Sphere* m_haumea;

    // Player Ship
    Mesh* m_mesh;

    // Cubemap
    Object* m_cubemap;

    Mesh* m_skybox;

};

#endif /* GRAPHICS_H */
