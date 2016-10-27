#include "Effects.hpp"
#include <iostream>

using Game::Effects;

Effects::Effects(const sf::Vector2u& screenSize) {
	texture.create(screenSize.x, screenSize.y);
}

sf::Shader& Effects::addShader(const std::string& name, sf::Shader::Type type) {
	if (!shaders[name].loadFromFile(name, type)) {
		std::cerr << "[Effects.cpp] Error: couldn't load shader from " << name << std::endl;
	} 
#ifndef RELEASE
	else std::cerr << "Loaded shader " << name << std::endl;
#endif
	return shaders[name];
}

void Effects::removeShader(const std::string& name) {
	shaders.erase(name);
}

void Effects::render(std::initializer_list<const sf::Drawable*> args) {
	texture.clear(sf::Color::Black);
	for (auto a : args)
		texture.draw(*a);
	texture.display();
}

void Effects::drawWithEffects(sf::RenderTarget& target) {
/*	for (auto& shader : shaders) {
		sf::RenderTexture tmpTex;
		tmpTex.create(texture.getSize().x, texture.getSize().y);

		sf::Sprite sprite(texture.getTexture());
		tmpTex.draw(sprite, &shader.second);
		tmpTex.display();
		
		sf::Sprite tmpSprite(tmpTex.getTexture());
		texture.draw(tmpSprite);
		texture.display();
	}*/

	sf::Sprite sprite(texture.getTexture());
	target.draw(sprite, &shaders["lighting.frag"]);
}
