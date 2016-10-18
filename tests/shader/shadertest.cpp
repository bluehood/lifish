#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>

#define MAX_LIGHTS 16

const int random_pool_size = 1000;
float random_pool[random_pool_size];

void fill_random_pool();
float flickerStep(float*, int);

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Shader");
	window.setVerticalSyncEnabled(true);

	sf::RenderTexture renderTex;
	renderTex.create(800, 600);

	sf::Texture bgt;
	bgt.loadFromFile("bg1.png");
	bgt.setRepeated(true);
	sf::Sprite bg(bgt);
	bg.setTextureRect(sf::IntRect(0, 0, 800, 600));

	sf::Texture playerTex;
	playerTex.loadFromFile("player1.png");
	sf::Sprite player(playerTex);
	player.setTextureRect(sf::IntRect(0, 0, 32, 32));
	player.setScale(10, 10);
	playerTex.setSmooth(false);

	sf::Shader shader;
	shader.loadFromFile("lighting2.frag", sf::Shader::Fragment);

	fill_random_pool();
	const int flickerLen = 10;
	float smoothing[flickerLen] = {0};
	for (int i = 0; i < flickerLen; ++i)
		flickerStep(smoothing, flickerLen);

	sf::Glsl::Vec2 light_pos[1] = {
		sf::Glsl::Vec2(400, 300)
	};
	sf::Glsl::Vec3 light_color[1] = {
		sf::Glsl::Vec3(1.0, 0.0, 0.4)
	};
	float light_intensity[1] = {
		0.3	
	};
	float light_radius[1] = {
		50.0
	};
	shader.setUniform("tex", sf::Shader::CurrentTexture);
	shader.setUniformArray("light_pos", light_pos, MAX_LIGHTS);
	shader.setUniformArray("light_color", light_color, MAX_LIGHTS);
	shader.setUniformArray("light_intensity", light_intensity, MAX_LIGHTS);
	shader.setUniformArray("light_radius", light_radius, MAX_LIGHTS);
	shader.setUniform("n_lights", 1);

	bool withShader = true;

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (ev.key.code) {
				case sf::Keyboard::Q:
					window.close();
					break;
				case sf::Keyboard::T:
					withShader = !withShader;
					break;
				}
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			light_pos[0].y += 5;
			shader.setUniformArray("light_pos", light_pos, MAX_LIGHTS);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			light_pos[0].y -= 5;
			shader.setUniformArray("light_pos", light_pos, MAX_LIGHTS);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			light_pos[0].x -= 5;
			shader.setUniformArray("light_pos", light_pos, MAX_LIGHTS);
		} 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			light_pos[0].x += 5;
			shader.setUniformArray("light_pos", light_pos, MAX_LIGHTS);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
			light_radius[0] += 0.5;
			shader.setUniformArray("light_radius", light_radius, MAX_LIGHTS);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
			light_radius[0] -= 0.5;
			shader.setUniformArray("light_radius", light_radius, MAX_LIGHTS);
		}

		// flickering
		light_intensity[0] = flickerStep(smoothing, flickerLen);
		shader.setUniformArray("light_intensity", light_intensity, MAX_LIGHTS);

		renderTex.clear();
		renderTex.draw(bg);
		renderTex.draw(player);
		renderTex.display();

		window.clear();
		sf::Sprite renderSprite(renderTex.getTexture());
		if (withShader)
			window.draw(renderSprite, &shader);
		else
			window.draw(renderSprite);

		window.display();
	}
}

void fill_random_pool() {
	for (int i = 0; i < random_pool_size; ++i) {
		random_pool[i] = float(rand())/RAND_MAX; 
	}
}

float flickerStep(float *smoothing, int len) {
	static int j = 0;
	float sum = 0;
	for (int i = 1; i < len; ++i) {
		smoothing[i-1] = smoothing[i];
		sum += smoothing[i-1];
	}
	smoothing[len-1] = random_pool[j];
	j = (j + 1) % random_pool_size;
	sum += smoothing[len-1];
	return sum / len;
}