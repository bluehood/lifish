#pragma once

#include <memory>
#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <SFML/System.hpp>
#include "Activable.hpp"
#include "WithOrigin.hpp"
#include "Stringable.hpp"

#include <iostream>

namespace lif {

class Component;

/**
 * Base class for game entities (walls, enemies, players, ...)
 */
class Entity : public lif::WithOrigin, public lif::Stringable {

	using CompKey = std::type_index;

	bool _initialized = false;


	template<class T>
	CompKey _compKey() const;

protected:
	std::map<CompKey, std::vector<std::shared_ptr<lif::Component>>> components;
	sf::Vector2f position;
	std::string _toString(unsigned short indent) const;

public:
	explicit Entity();

	explicit Entity(const sf::Vector2f& pos);
	virtual ~Entity();

	template<class T>
	T* addComponent(T* comp);

	template<class Alias, class Real>
	void aliasComponent(const Real& comp);

	template<class T>
	T* get() const;

	template<class T>
	std::vector<T*> getAll() const;

	template<class T>
	std::vector<T*> getAllRecursive() const;

	template<class T>
	std::shared_ptr<T> getShared() const;

	template<class T>
	std::vector<std::shared_ptr<T>> getAllShared() const;

	template<class T>
	std::vector<std::shared_ptr<T>> getAllRecursiveShared() const;

	virtual sf::Vector2f getPosition() const; 
	virtual void setPosition(const sf::Vector2f& p); 
	void translate(const sf::Vector2f& p);

	bool isAligned(const char axis = 'b') const;

	/** Called after the constructor; all components should have been already
	 *  added at this time.
	 *  @return this
	 */
	virtual lif::Entity* init();
	/** Called every frame */
	virtual void update();

	/** Implements WithOrigin */
	virtual void setOrigin(const sf::Vector2f& origin) override;
	
	/** Implements Stringable */
	virtual std::string toString() const override;
};

#define COMP_NOT_UNIQUE \
	static constexpr bool requiredUnique() { return false; }

/**
 * A generic component of a game entity. Inherit this to add behavior.
 */
class Component : public lif::Entity, public lif::Activable {
protected:
	lif::Entity& owner;

public:
	/** If true, adding more than a component of this type to an Entity
	 *  will raise a logic_error.
	 */
	static constexpr bool requiredUnique() { return true; }

	explicit Component(lif::Entity& owner);

	/** Gets the owner of this component */
	const lif::Entity& getOwner() const { return owner; }

	/** Gets the owner of this component (non-const) */
	lif::Entity& getOwnerRW() const { return owner; }
};

#include "Entity.inl"

}
