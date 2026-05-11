#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Init Light
	m_light = new Light(m_camera->GetView());
	m_light->updatePosition(m_camera->GetView(), glm::vec3(10., 0., 0.));

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	if (!materialSetup()) {
		printf("Some shader mats not working!\n");
	}

	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "SpaceShip-1\\SpaceShip-1.obj", "SpaceShip-1\\SpaceShip-1.png");

	// The Sun
	m_sun = new Sphere(64, "Planetary Textures\\2k_sun.jpg");

	// Planets
	m_mars = new Sphere(48, "Planetary Textures\\Mars.jpg", "Planetary Textures\\Mars-n.jpg");
	m_mercury = new Sphere(64, "Planetary Textures\\Mercury.jpg", "Planetary Textures\\Mercury-n.jpg");

	//// Earth System
	m_earth = new Sphere(64, "Planetary Textures\\2k_earth_daymap.jpg", "Planetary Textures\\2k_earth_daymap-n.jpg");
	m_moon = new Sphere(48, "Planetary Textures\\2k_Moon.jpg", "Planetary Textures\\2k_Moon-n.jpg");

	m_venus = new Sphere(64, "Planetary Textures\\Venus.jpg", "Planetary Textures\\Venus-n.jpg");
	m_neptune = new Sphere(64, "Planetary Textures\\Neptune.jpg", "Planetary Textures\\Neptune-n.jpg");
	m_saturn = new Sphere(64, "Planetary Textures\\Saturn.jpg");
	m_jupiter = new Sphere(48, "Planetary Textures\\Jupiter.jpg", "Planetary Textures\\Jupiter-n.jpg");
	m_uranus = new Sphere(64, "Planetary Textures\\Uranus.jpg", "Planetary Textures\\Uranus-n.jpg");

	//// Asteroid Belt and other bodies
	//m_ceres = new Sphere(20, "Planetary Textures\\Ceres.jpg", "Planetary Textures\\Ceres-n.jpg");
	//m_eris = new Sphere(20, "Planetary Textures\\Eris.jpg", "Planetary Textures\\Eris-n.jpg");
	//m_haumea = new Sphere(20, "Planetary Textures\\Haumea.jpg", "Planetary Textures\\Haumea-n.jpg");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}
// Big planetary bodies
void Graphics::HierarchicalUpdateSystem1(double dt) {

	/// NOTES: uncomment objects in init() to make sure they render
	// Unlike in previous hmws, I assign modelstack.top a new value to go by the sun and not other planets
	// If program breaks its because normals arent given to certain objects, check render()

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;

	// position of the Sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // world origin
	modelStack.push(modelStack.top());		// The sun origin
	modelStack.top() *= glm::rotate(glm::mat4(1.0f), (float)dt, glm::vec3(0.f, 1.f, 0.f));
	modelStack.top() *= glm::scale(glm::vec3(.75, .75, .75));
	if (m_sun != NULL)
		m_sun->Update(modelStack.top());
	modelStack.pop(); // back to sun's positional transformation

	// position of Mercury
	speed = { 1., 1., 1. };
	dist = { 1.25, 0, 1.25 };
	rotVector = { 1.,1.,1. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2,.2,.2 };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());			// store planet coordinate
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury != NULL)

		m_mercury->Update(modelStack.top());
	modelStack.pop();		// back to planet's positional coordinate (remove the rotration, scale)

	// position of Venus
	speed = { 6, 6, 6 };
	dist = { 1.9, 0, 1.9 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .25f, .25f, .25f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_venus != NULL)
		m_venus->Update(modelStack.top());
	modelStack.pop();

	// position of Earth
	speed = { 2, 2, 2 };
	dist = { 4.5, 0, 4.5 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .4f, .4f, .4f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_earth != NULL)
		m_earth->Update(modelStack.top());
	modelStack.pop();

	// position of Moon
	speed = { 2, 2, 2 };
	dist = { 1.5, 1, 1.5 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .1f, .1f, .1f };
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_moon != NULL)
		m_moon->Update(modelStack.top());
	modelStack.pop();

	// position of Mars
	speed = { 2, 2, 2 };
	dist = { 9.5, 0, 9.5 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .4f, .4f, .4f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_mars != NULL)
		m_mars->Update(modelStack.top());
	modelStack.pop();

	// position of Jupiter
	speed = { 1.5, 1.5, 1.5 };
	dist = { 20.0, 0, 20.0 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { 1.5f, 1.5f, 1.5f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_jupiter != NULL)
		m_jupiter->Update(modelStack.top());
	modelStack.pop();

	// position of Saturn
	speed = { 1.2, 1.2, 1.2 };
	dist = { 50.0, 0, 50.0 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { 1.4f, 1.4f, 1.4f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_saturn != NULL)
		m_saturn->Update(modelStack.top());
	modelStack.pop();

	// position of Uranus
	speed = { 1.3, 1.3, 1.3 };
	dist = { 90.0, 0, 90.0 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { 1.4f, 1.4f, 1.4f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_uranus != NULL)
		m_uranus->Update(modelStack.top());
	modelStack.pop();

	// position of Neptune
	speed = { 1.1, 1.1, 1.1 };
	dist = { 120.0, 0, 120.0 };
	rotVector = { 1.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { 1.4f, 1.4f, 1.4f };
	modelStack.top() = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(modelStack.top());
	modelStack.top() *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	modelStack.top() *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_neptune != NULL)
		m_neptune->Update(modelStack.top());
	modelStack.pop();


	//modelStack.pop(); 	//back to the planet coordinate

	modelStack.pop(); 	// back to the world coordinate

}

void Graphics::HierarchicalUpdateSystem2(double dt) {

	// Update your animation for the solar system here.

}

void Graphics::HierarchicalUpdateSystem3(double dt) {

	// Update your animation for the solar system here.

}

void Graphics::moveShip(glm::vec3 moveDir, double dt)
{
	/// Calc matrix here
	/*glm::mat4 currentMatrix = m_mesh->GetModel();
	glm::mat4 tmat = glm::translate(glm::mat4(1.f), glm::vec3(0,0,moveDir.z));
	glm::mat4 rmat = glm::rotateY(glm::mat4(1.f), glm::vec3(0,1,0));
	glm::mat4 smat;*/
}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			//m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	m_hasN = m_shader->GetUniformLocation("hasNormalMap");
	if (m_sun != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_sun->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->getShaderProgram());
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);
		}
		if (m_sun->hasNormal) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_sun->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUseProgram(m_shader->getShaderProgram());
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	if (m_mars != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_mars->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUseProgram(m_shader->getShaderProgram());
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_mars->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_mars->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUseProgram(m_shader->getShaderProgram());
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_jupiter != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_jupiter->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_jupiter->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_uranus != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_uranus->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_uranus->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_uranus->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_venus != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_venus->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_venus->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_venus->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_mercury != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_mercury->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_mercury->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_mercury->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_earth != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_earth->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		if (m_earth->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_earth->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_earth->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_earth->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_saturn != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_saturn->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_saturn->hasNormal) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_saturn->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Render Jupiter
	if (m_moon != NULL) {
		glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetView() * m_moon->GetModel())))));
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_moon->GetModel()));
		if (m_moon->getTextureID()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_moon->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("samp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			glUniform1i(m_hasN, false);

		}
		if (m_moon->getNormalID()) {
			glActiveTexture(GL_TEXTURE1);
			//cout << m_jupiter->getNormalID() << endl;
			glBindTexture(GL_TEXTURE_2D, m_moon->getNormalID());
			GLuint sampler = m_shader->GetUniformLocation("samp1");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler-N Not found not found\n");
			}
			glUniform1i(sampler, 1);
			glUniform1i(m_hasN, true);
		}
		glUseProgram(m_shader->getShaderProgram());
		m_moon->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	m_normalMatrix = m_shader->GetUniformLocation("normMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_normalMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_normal"); /// TODO: check if right???
	if (m_colorAttrib == -1)
	{
		printf("v_normal attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("texCoord");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	/// New Shader Vars for normals and lighting
	globalAmbLoc = m_shader->GetUniformLocation("GlobalAmbient");
	if (globalAmbLoc == INVALID_UNIFORM_LOCATION) {
		printf("globalAmbLoc uniform not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->getShaderProgram(), globalAmbLoc, 1, m_light->getGlobalAmbient());

	lightALoc = m_shader->GetUniformLocation("light.ambient");
	if (lightALoc == INVALID_UNIFORM_LOCATION) {
		printf("lightALoc uniform not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->getShaderProgram(), lightALoc, 1, m_light->getLightAmbient());

	lightDLoc = m_shader->GetUniformLocation("light.diffuse");
	if (lightDLoc == INVALID_UNIFORM_LOCATION) {
		printf("lightDLoc uniform not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->getShaderProgram(), lightDLoc, 1, m_light->getDiffuse());

	lightSLoc = m_shader->GetUniformLocation("light.spec");
	if (lightSLoc == INVALID_UNIFORM_LOCATION) {
		printf("lightSLoc uniform not found\n");
		anyProblem = false;
	}
	glProgramUniform4fv(m_shader->getShaderProgram(), lightSLoc, 1, m_light->getSpecular());

	lightPosLoc = m_shader->GetUniformLocation("light.position");
	if (lightPosLoc == INVALID_UNIFORM_LOCATION) {
		printf("lightPosLoc uniform not found\n");
		anyProblem = false;
	}
	glProgramUniform3fv(m_shader->getShaderProgram(), lightPosLoc, 1, m_light->getLightPosViewSpace());

	return anyProblem;
}

bool Graphics::materialSetup()
{
	bool anyProblem = true;

	// Material 1
	float matAmbient[4] = { 0.2, 0.2, 0.2, 0.2 };
	float matDiff[4] = { 1.0, 1.0, 1.0, 1.0 };
	float matSpec[4] = { 1.0, 1.0, 1.0, 1.0 };
	float matShininess = 20.0;

	///TODO: add error check later maybe
	GLuint mAmbLoc = m_shader->GetUniformLocation("material.ambient");
	glProgramUniform4fv(m_shader->getShaderProgram(), mAmbLoc, 1, matAmbient);

	GLuint mDiffLoc = m_shader->GetUniformLocation("material.diffuse");
	glProgramUniform4fv(m_shader->getShaderProgram(), mDiffLoc, 1, matDiff);

	GLuint mSpecLoc = m_shader->GetUniformLocation("material.spec");
	glProgramUniform4fv(m_shader->getShaderProgram(), mSpecLoc, 1, matSpec);

	GLuint mShineLoc = m_shader->GetUniformLocation("material.shininess");
	glProgramUniform1f(m_shader->getShaderProgram(), mShineLoc, matShininess);


	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

