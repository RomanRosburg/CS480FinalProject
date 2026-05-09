#pragma once
#include "graphics_headers.h"

class Light
{
public:
	Light(glm::mat4 viewMatrix);
	Light(glm::mat4 viewMatrix, glm::vec3 lightPosition, glm::vec4 lightDiffuse, glm::vec4 lightAmb, glm::vec4 lightSpecular, glm::vec4 gAmb);

	void updatePosition(glm::mat4 viewMatrix, glm::vec3 lightPosition);

	float* getDiffuse() { return m_lightDiffuse; }
	float* getLightAmbient() { return m_lightlAmbient; }
	float* getSpecular() { return m_lightSpecular; }
	float* getGlobalAmbient() { return m_globalAmbient; }
	float* getLightPosViewSpace() { return m_lightPositionViewSpace; }

private:
	// Positioning
	glm::vec3 m_lightPosition;
	float m_lightPositionViewSpace[3];

	// Coloring/Properties (r, g, b, a)
	float m_lightDiffuse[4];
	float m_lightlAmbient[4];
	float m_lightSpecular[4];
	float m_globalAmbient[4];
};

