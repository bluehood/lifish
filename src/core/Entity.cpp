#include "Entity.hpp"
#include "GameCache.hpp"
#include "core.hpp"
#include "Component.hpp"
#include "utils.hpp"
#include <sstream>
#include <iostream>

// Note: in theory, this should check for HAVE_CXA_DEMANGLE.
// The GCC version that I'm using, though, despite being pretty recent (6.2.1),
// does not define that macro, so I just check that we're using GCC and hope
// it's not a prehistoric version.
// In case of compilation problems, just change this directive and give up
// having class names demangled in toString().
#ifdef __GNUG__
#	include <cxxabi.h>
#	include <cstdlib>

static std::string demangle(const char *name) {
	int status;
	char *res = abi::__cxa_demangle(name, NULL, NULL, &status);
	if (status == 0) {
		std::string s(res);
		free(res);
		return s;
	}
	return std::string(name);
}

#	define DEMANGLE(name) demangle(name)
#else
#	define DEMANGLE(name) name
#endif

using lif::Entity;

Entity::Entity() {}

Entity::Entity(const sf::Vector2f& pos)
	: position(pos)
{}

Entity::~Entity() {}

void Entity::setOrigin(const sf::Vector2f& origin) {
	WithOrigin::setOrigin(origin);
	for (auto& pair : components)
		for (auto& c : pair.second)
			c->setOrigin(origin);
}

bool Entity::isAligned(const char axis) const {
	switch (axis) {
		case 'x': return static_cast<int>(position.x) % lif::TILE_SIZE == 0;
		case 'y': return static_cast<int>(position.y) % lif::TILE_SIZE == 0;
	}
	return static_cast<int>(position.x) % lif::TILE_SIZE == 0
		&& static_cast<int>(position.y) % lif::TILE_SIZE == 0;
}

lif::Entity* Entity::init() {
	std::cout << "initializing " << typeid(*this).name() << std::endl;
	if (_initialized) return this;

	for (auto it = components.begin(); it != components.end(); ++it) {
		for (auto& c : it->second)
			c->init();
	}
	
	_initialized = true;
	return this;
}

void Entity::update() {
	for (auto& pair : components)
		for (auto& c : pair.second)
			if (c->isActive())
				c->update();
}

std::string Entity::_toString(unsigned short indent) const {
	std::stringstream ss;
	auto put_indent = [&ss] (unsigned short indent) -> std::stringstream& { 
		for (unsigned i = 0; i < indent; ++i) 
			ss << "    ";
		return ss;
	};
	put_indent(indent) << "[" << DEMANGLE(typeid(*this).name())
		<< " @ " << position << " / " << lif::tile(position)
		<< " ~ aligned = " << isAligned() << "]";
	if (components.size() > 0) {
		ss << "\r\n";
		put_indent(indent) << "{\r\n";
		for (const auto& pair : components) {
			for (const auto& c : pair.second)
				ss << c->_toString(indent + 1) << "\r\n";
		}
		put_indent(indent) << "}";
	}
	return ss.str();
}

sf::Vector2f Entity::getPosition() const { 
	return position;
} 

void Entity::setPosition(const sf::Vector2f& p) { 
	position = p;
}

void Entity::translate(const sf::Vector2f& offset) {
	setPosition(getPosition() + offset);
}

std::string Entity::toString() const {
	return _toString(0);
}
