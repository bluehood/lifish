#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace Game {

class Effects : private sf::NonCopyable {
	sf::RenderTexture texture;
	std::map<std::string, sf::Shader> shaders;

public:
	explicit Effects(const sf::Vector2u& size);

	sf::Shader& addShader(const std::string& fname, sf::Shader::Type type);
	void removeShader(const std::string& fname);

	void render(std::initializer_list<const sf::Drawable*> args);
	void drawWithEffects(sf::RenderTarget& target);
};

}
