#if 1
#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/renderer2d.h"
#include "src/graphics/batchrenderer2d.h"

#include "src/graphics/sprite.h"

#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"

#include "src/graphics/layers/group.h"
#include "src//graphics/layers/layer.h"

#include "src/graphics/font_manager.h"
#include "src/graphics/label.h"
#include "src/graphics/texture.h"

#include "src/audio/sound_manager.h"

float GenerateRandom(float min, float max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	if (min > max)
	{
		std::swap(min, max);
	}
	return min + rand() * (max - min) / RAND_MAX  ;
}

int main()
{
	using namespace Engine;
	using namespace graphics;
	using namespace audio;
	using namespace maths;

	Window* window;
	Layer* layer;
	Shader* shader;

	window = new Window("TestGame", 1600, 900);
	FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
	shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 0.0f));

	Texture* face_textures[] = { new Texture("face.jpg") };

	float pos_x = GenerateRandom(-16.0f, 15.0f);
	float pos_y = GenerateRandom(-09.0f, 08.0f);

	Sprite* face_sprite = new Sprite(pos_x, pos_y, 1, 1, 0xffffffff);
	layer->add(face_sprite);

	Label* fps = new Label("", -15.5f, 8.0f, 0xffffffff);
	layer->add(fps);

	SoundManager::add(new Sound("bg_music", "mystery.wav"));
	//SoundManager::add(new Sound("Test", "surprise.wav"));
	SoundManager::add(new Sound("yay", "clapping.wav"));

	SoundManager::get("bg_music")->loop();
	Timer time;
	float timer = 0;
	unsigned int frames = 0;
	float t = 0.0f;
	while (!window->closed())
	{
		t += 0.001f;
		window->clear();
		double x, y;
		window->getMousePosition(x, y);
		
		shader->setUniform2f("light_pos", vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
		layer->render();

		if (window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT)) {
			SoundManager::get("bg_music")->stop();
			SoundManager::get("yay")->stop();
			SoundManager::get("yay")->loop();
		}

		window->update();
		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			fps->text = std::to_string(frames) + "fps";
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	for (int i = 0; i < 1; i++)
		delete face_textures[i];

	return 0;
}
#endif