#include "Component.hpp"
#include "Entity.hpp"

using lif::Component;

Component::Component(lif::Entity& owner)
	: owner(owner)
{}
