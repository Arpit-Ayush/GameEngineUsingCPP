#if 0
#include "../src/engine.h"

using namespace Engine;
using namespace graphics;

class Game : public Engine{
private:
	Window* window;
	Layer* layer;
	Label* fps;
	Sprite* sprite;
	Shader* shader;
public:
	Game() {

	}

	~Game() {
		delete layer;
	}

	void init() override {
		window = createWindow("TestGame", 1600, 900);
		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
		shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
		layer = new Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		Texture* wel_txt = new Texture("../wel_text.jpg");
		sprite = new Sprite(0.0f, 0.0f, 5, 5, wel_txt);
		layer->add(sprite);

		fps = new Label("", -15.5f, 8.0f, 0xffffffff);
		layer->add(fps);
	}

	void tick() override{
		fps->text = std::to_string(getFPS()) + " fps";
		std::cout << getUPS() << " ups, " << getFPS() << " fps" << std::endl;
	}

	void update() override {
		double x, y;
		window->getMousePosition(x, y);

		shader->setUniform2f("light_pos", maths::vec2((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
		layer->render();
	}

	void render() override {
		layer->render();
	}
};

int main() {
	Game game;
	game.start();
	return 0;
}
#endif