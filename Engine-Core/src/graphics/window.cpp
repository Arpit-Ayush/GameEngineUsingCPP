#include "window.h"
namespace Engine {
	namespace graphics {


		Window::Window(const char* title, int width, int height)
		{
			m_Title = title;
			m_Width = width;
			m_Height = height;
			if (!init())
				glfwTerminate();

			FontManager::add(new Font("SourceSans3-Light", "SourceSans3-Light.ttf", 32));
			audio::SoundManager::init();

			for (int i = 0; i < MAX_KEYS; ++i) {
				m_Keys[i] = false;
				m_KeyState[i] = false;
				m_KeyTyped[i] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; ++i) {
				m_MouseButtons[i] = false;
				m_MouseState[i] = false;
				m_MouseClicked[i] = false;
			}
		}
		Window::~Window()
		{
			audio::SoundManager::clean();
			FontManager::clean();
			glfwTerminate();
		}
		bool Window::init()
		{
			if (!glfwInit())
			{
				std::cout << "Failed to initialize GLFW!" << std::endl;
				return false;
			}
			m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
			if (!m_Window)
			{
				std::cout << "Failed to create GLFW window!" << std::endl;
				return false;
			}
			glfwMakeContextCurrent(m_Window);
			glfwSetWindowUserPointer(m_Window, this);
			glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
				{
					glViewport(0, 0, width, height);
					Window* win = (Window*)glfwGetWindowUserPointer(window);
					win->m_Width = width;
					win->m_Height = height;
				});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->m_MouseButtons[button] = action != GLFW_RELEASE;
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow * window, int key, int scancode, int action, int mods) {
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->m_Keys[key] = action != GLFW_RELEASE;
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
				Window* win = (Window*)glfwGetWindowUserPointer(window);
				win->mx = xpos;
				win->my = ypos;
			});

			glfwSwapInterval(0);

			if (glewInit() != GLEW_OK)
			{
				std::cout << "Could not initialize GLEW!" << std::endl;
				return false;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
			return true;
		}

		bool Window::isKeyPressed(unsigned int keycode) const {
			if (keycode >= MAX_KEYS)
				return false;

			return m_Keys[keycode];
		}

		bool Window::isKeyTyped(unsigned int keycode) const
		{
			if (keycode >= MAX_KEYS)
				return false;

			return m_KeyTyped[keycode];
		}

		bool Window::isMouseButtonPressed(unsigned int button) const {
			if (button >= MAX_BUTTONS)
				return false;

			return m_MouseButtons[button];
		}

		bool Window::isMouseButtonClicked(unsigned int button) const
		{
			if (button >= MAX_BUTTONS)
				return false;

			return m_MouseClicked[button];
		}

		void Window::getMousePosition(double& x, double& y) const{
			x = mx;
			y = my;
		}

		void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		void Window::update()
		{
			for (int i = 0; i < MAX_KEYS; i++)
				m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];

			for (int i = 0; i < MAX_BUTTONS; i++)
				m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

			memcpy(m_KeyState, m_Keys, MAX_KEYS);
			memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
				std::cout << "OpenGL Error: " << error << std::endl;
			glfwPollEvents();
			glfwSwapBuffers(m_Window);

			audio::SoundManager::update();
		}
		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_Window) == 1;
		}
}
}