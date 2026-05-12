

#include "engine.h"
#include "glm/ext.hpp"

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
  // Default to exploration mode
  exploration = true;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }
  
  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Mouse input should be tied to this engine
  Engine::current = this;
  m_graphics->getCamera()->setX(m_WINDOW_WIDTH / 2);
  m_graphics->getCamera()->setY(m_WINDOW_HEIGHT / 2);
  glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallback);
  glfwSetScrollCallback(m_window->getWindow(), mouseScrollCallback);
  glfwSetKeyCallback(m_window->getWindow(), keyCallback);

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      ProcessInput();
      Display(m_window->getWindow(), glfwGetTime());
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    if (exploration) {
        /// Ship Movement relative to camera angle
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            m_graphics->moveShip(m_graphics->getCamera()->getFront(), getDT() / -5.0f);
        }
        else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            m_graphics->moveShip(m_graphics->getCamera()->getFront(), getDT() / 5.0f);
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            m_graphics->moveShip(m_graphics->getCamera()->getRight(), getDT() / -5.0f);
        }
        else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            m_graphics->moveShip(m_graphics->getCamera()->getRight(), getDT() / 5.0f);
        }
    }
    else {
        double x = m_graphics->getCamera()->getX();
        double y = m_graphics->getCamera()->getY();
        // Panning about a target object
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
            m_graphics->getCamera()->mouseLook(
                x, // No change
                y + 10
            );
        }
        else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
            m_graphics->getCamera()->mouseLook(
                x, // No change
                y - 10
            );
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
            m_graphics->getCamera()->mouseLook(
                x + 10,
                y // No change
            );
        }
        else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
            m_graphics->getCamera()->mouseLook(
                x - 10,
                y // No change
            );
        }
        /* Planetary observation lights
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_F) == GLFW_PRESS) {

        }
        else if (glfwGetKey(m_window->getWindow(), GLFW_KEY_F) == GLFW_RELEASE) {

        }*/
    }
    m_graphics->getCamera()->setPos(
        glm::vec3((*m_graphics->getCamera()->getTarget())[3])
            - m_graphics->getCamera()->getZ() * m_graphics->getCamera()->getFront()
    );
}

double Engine::getDT()
{
  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;
  //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
  //timeval t;
  //gettimeofday(&t, NULL);
  //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  //return ret;
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdateSystem1(time);
}

Engine* Engine::current = NULL;

void Engine::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Mouse should be free to select celestial bodies in observation mode
    if (current->exploration) {
        current->m_graphics->getCamera()->mouseLook(xpos, ypos);
        /*current->m_graphics->getShip()->Update(
            glm::rotate(
                glm::mat4(1.0f),
                (float) current->m_graphics->getCamera()->getYaw(),
                glm::vec3(0.0f, 1.0f, 0.0f)
            ) * glm::rotate(
                glm::mat4(1.0f),
                (float)current->m_graphics->getCamera()->getPitch(),
                glm::vec3(0.0f, 0.0f, 1.0f)
            )
        );*/
    }
}

// Change the distance between the camera's target object and the camera
void Engine::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (current->m_graphics->getCamera()->getTarget() == NULL) {
        return;
    }
    double z = current->m_graphics->getCamera()->getZ();
    if ((z > 3 && z < 300)
        || (z <= 3 && yoffset < 0)
        || (z >= 300 && yoffset > 0)) {
        current->m_graphics->getCamera()->setZ(z - yoffset);
    }
}

// Callback for keys that should only be processed once rather than continuously
void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // A way to toggle planet observation mode
    if (glfwGetKey(current->m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        current->exploration = !current->exploration;
        if (current->exploration) {
            current->m_graphics->getCamera()->updateFollow(current->m_graphics->getShip()->GetDynamicModel());
        }
        else {
            current->m_graphics->getCamera()->updateFollow(current->m_graphics->getSun()->GetDynamicModel());
            // Set camera position so it's still centered on the body
            glm::vec3 target = glm::vec3((*current->m_graphics->getCamera()->getTarget())[3]);
            glm::vec3 pos = current->m_graphics->getCamera()->getPos();
            current->m_graphics->getCamera()->setPos(target - glm::distance(target, pos) * current->m_graphics->getCamera()->getFront());
        }
    }

    // When in observation mode, switch celestial bodies
    if (glfwGetKey(current->m_window->getWindow(), GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!current->exploration && current->m_graphics->getCamera()->getTarget() != NULL) {
            current->m_graphics->getCamera()->updateFollow(current->m_graphics->observeNext());
        }
    }
}