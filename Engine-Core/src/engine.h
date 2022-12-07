#if 0
#pragma once

#include "graphics/label.h"
#include "graphics/sprite.h"
#include "graphics/renderer2d.h"
#include "graphics/batchrenderer2d.h"
#include "graphics/window.h"
#include "graphics/texture.h"
#include "graphics/layers/layer.h"

#include "maths/maths.h"
#include "utils/timer.h"

namespace Engine {

	class Engine {
	private:
		graphics::Window* m_Window;
		Timer* m_Timer;
		unsigned int m_FramesPerSecond, m_UpdatesPerSecond;
	protected:
		Engine() 
			: m_FramesPerSecond(0), m_UpdatesPerSecond(0)
		{

		}

		virtual ~Engine(){
			delete m_Timer;
			delete m_Window;
		}

		graphics::Window* createWindow(const char* name, int width, int height) {
			m_Window = new graphics::Window(name, width, height);
			return m_Window;
		}
	public:

		void start() {
			init();
			run();
		}

	protected:
		//will run once upon initialization
		virtual void init() = 0;
		//will run once every second
		virtual void tick() {};
		//will run 60 times every second
		virtual void update() {};
		//will run as fast as possible
		virtual void render() = 0;

		const unsigned int getFPS() const { return m_FramesPerSecond; }
		const unsigned int getUPS() const { return m_UpdatesPerSecond; }
	private:
		void run() {
			m_Timer = new Timer();
			float timer = 0.0f;
			float updateTimer = 0.0f;
			float updateTick = 1.0f / 60.0f;
			unsigned int frames = 0;
			unsigned int updates = 0;

			while (!m_Window->closed()) {
				m_Window->clear();
				if (m_Timer->elapsed() - updateTimer > updateTick)
				{
					update();
					updates++;
					updateTimer += updateTick;
				}
				render();
				frames++;
				m_Window->update();
				if (m_Timer->elapsed() - timer > 1.0f)
				{
					timer += 1.0f;
					m_FramesPerSecond = frames;
					m_UpdatesPerSecond = updates;
					frames = 0;
					updates = 0;
					tick();
				}
			}
		}
	};

}
#endif