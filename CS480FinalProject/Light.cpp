#include "Light.h"

Light::Light(glm::mat4 viewMatrix)
{
	m_lightPosition = glm::vec3(0.,0.,0.);

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;

	// Ambient
	m_lightlAmbient[0] = 1.0;
	m_lightlAmbient[1] = 1.0;
	m_lightlAmbient[2] = 1.0;
	m_lightlAmbient[3] = 0.0;

	// Diffuse
	m_lightDiffuse[0] = 1.0;
	m_lightDiffuse[1] = 1.0;
	m_lightDiffuse[2] = 1.0;
	m_lightDiffuse[3] = 1.0;

	// Specular
	m_lightSpecular[0] = 0.0;
	m_lightSpecular[1] = 0.0;
	m_lightSpecular[2] = 1.0;
	m_lightSpecular[3] = 1.0;

	// Light Ambient
	m_lightlAmbient[0] = 1.0;
	m_lightlAmbient[1] = 1.0;
	m_lightlAmbient[2] = 1.0;
	m_lightlAmbient[3] = 1.0;

	// Global Ambient
	m_globalAmbient[0] = 0.1;
	m_globalAmbient[1] = 0.1;
	m_globalAmbient[2] = 0.1;
	m_globalAmbient[3] = 1.0;

}

Light::Light(glm::mat4 viewMatrix, glm::vec3 lightPosition, glm::vec4 lightDiffuse, glm::vec4 lightAmb, glm::vec4 lightSpecular, glm::vec4 gAmb)
{
	m_lightPosition = lightPosition;

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;

	// Ambient
	m_lightlAmbient[0] = lightAmb.r;
	m_lightlAmbient[1] = lightAmb.g;
	m_lightlAmbient[2] = lightAmb.b;
	m_lightlAmbient[3] = lightAmb.a;

	// Diffuse
	m_lightDiffuse[0] = lightDiffuse.r;
	m_lightDiffuse[1] = lightDiffuse.g;
	m_lightDiffuse[2] = lightDiffuse.b;
	m_lightDiffuse[3] = lightDiffuse.a;

	// Specular
	m_lightSpecular[0] = lightSpecular.r;
	m_lightSpecular[1] = lightSpecular.g;
	m_lightSpecular[2] = lightSpecular.b;
	m_lightSpecular[3] = lightSpecular.a;

	// Light Ambient
	m_lightlAmbient[0] = lightAmb[0];
	m_lightlAmbient[1] = lightAmb[1];
	m_lightlAmbient[2] = lightAmb[2];
	m_lightlAmbient[3] = lightAmb[3];

	// Global Ambient
	m_globalAmbient[0] = gAmb.r;
	m_globalAmbient[1] = gAmb.g;
	m_globalAmbient[2] = gAmb.b;
	m_globalAmbient[3] = gAmb.a;
}

void Light::updatePosition(glm::mat4 viewMatrix, glm::vec3 lightPosition)
{
	m_lightPosition = lightPosition;

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).x;
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).y;
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(m_lightPosition, 1.0f))).z;
}
