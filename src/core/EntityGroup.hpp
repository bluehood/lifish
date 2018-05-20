#pragma once

#include <memory>
#include <vector>
#include <array>
#include <iterator>
#include <unordered_set>
#include <functional>
#include <type_traits>
#include <algorithm>
#include <SFML/System/NonCopyable.hpp>
#include "Entity.hpp"
#include "Temporary.hpp"
#include "Killable.hpp"
#include "Moving.hpp"
#include "Collider.hpp"
#include "Fixed.hpp"
#ifdef MULTITHREADED
#	include <mutex>
#endif

namespace lif {

class CollisionDetector;
class LevelRenderer;

/**
 * A container for Entities, providing convenient methods for operating
 * on all or a specific type of them.
 */
class EntityGroup final : private sf::NonCopyable {

	template<typename F, typename...Args>
	using result_t = typename std::result_of<F(lif::Entity*, Args&&...)>::type;

	template<typename F, typename R, typename...Args>
	using returns_t = typename std::enable_if<std::is_same<
		result_t<F, Args...>, 
		R
		>::value, std::nullptr_t>::type;


	bool alreadyPrunedThisUpdate = false,
	     alreadyCheckedThisUpdate = false;

#ifdef MULTITHREADED
	mutable std::mutex mutex;
#endif

	/** All the entities (owning references) */
	std::vector<std::shared_ptr<lif::Entity>> entities;

	/** The colliders of entities which have one */
	std::vector<std::weak_ptr<lif::Collider>> collidingEntities;

	/** The vector of the killable entities, which ought to be removed when
	 *  their `isKilled()` method yields true.
	 */
	std::vector<std::weak_ptr<lif::Killable>> killables;

	/** The vector of Killable entities who are being destroyed (those whose isKilled() is
	 *  true but isKillInProgress() is true as well).
	 */
	std::vector<std::weak_ptr<lif::Killable>> dying;


	/** Removes any killed entity from all internal collections (including the main one) and destroys them.
	 *  If its `isKillInProgress()` is true, puts it in `dying`
	 *  instead of immediately destroing it (it is not removed from `entities` until it's finalized)
	 */
	void _checkKilled();
	/** Removes any entity in `dying` which has `isKillInProgress() == false` and destroys them (this also
	 *  removes them from the main collections).
	 */
	void _checkDead();

	/** Iterate over aux collections and remove all expired weak pointers.
	 *  Note that, differently from the `_check*` methods, the `_prune*` ones do NOT
	 *  affect the main `entities` collection.
	 */
	void _pruneAll();
	void _pruneColliding();

	lif::Entity* _putInAux(lif::Entity *entity);
public:
	static constexpr bool APPLY_PROCEED = false;
	static constexpr bool APPLY_EXIT = true;

	/**
	 * Constructs the EntityGroup as the owner of its entities.
	 */
	explicit EntityGroup();

	/** Applies a function to all entities.
	 *  If the passed function returns APPLY_EXIT for an Entity,
	 *  this function will return without processing additional entities.
	 */
	template<typename F, typename...Args, returns_t<F, bool, Args...> = nullptr>
	void apply(const F& func, Args&&... args) {
		for (auto& e : entities)
			if (func(e.get(), std::forward<Args>(args)...))
				return;
	}

	/** Applies a function to all entities. */
	template<typename F, typename...Args, returns_t<F, void, Args...> = nullptr>
	void apply(const F& func, Args&&... args) {
		for (auto& e : entities)
			func(e.get(), std::forward<Args>(args)...);
	}

	/** @see apply */
	template<typename F, typename...Args, returns_t<F, bool, Args...> = nullptr>
	auto apply(const F& func, Args&&... args) const {
		for (auto& e : entities)
			if (func(e.get(), std::forward<Args>(args)...))
				return;
	}

	/** @see apply */
	template<typename F, typename...Args, returns_t<F, void, Args...> = nullptr>
	auto apply(const F& func, Args&&... args) const {
		for (auto& e : entities)
			func(e.get(), std::forward<Args>(args)...);
	}

	/** Adds an entity to this group. */
	lif::Entity* add(lif::Entity *entity);

	/** @see add */
	template<typename T>
	lif::Entity* add(std::shared_ptr<T> entity);

	/** Removes an entity from all internal collections. */
	void remove(const lif::Entity& entity);
	/** @see remove */
	void remove(const std::shared_ptr<const lif::Entity>& entity);

	/** Removes all entities from this EntityGroup. */
	void clear();

	/** @return the number of entities of type T in this group */
	template<typename T>
	size_t size() const;
	/** @return the total number of entitiies in this group */
	size_t size() const { return entities.size(); }

	/** Explicitly request that all expired weak_ptr's are pruned. This is done
	 *  automatically in updateAll() if this method hasn't been called since latest update.
	 *  Note that calling this method does NOT remove any killed or dead entity from the main
	 *  collection: that is only done in `updateAll`, so one can still refer to an "almost-dead"
	 *  entity (i.e. one that `isKilled()` and has `isKillInProgress() == false`) to do
	 *  something in its last update cycle between a call to `validate()` and `updateAll()`.
	 */
	void validate();
	/** Explicitly request that the internal helper lists are updated. */
	void checkAll();
	/** Calls `update` for every entity in this group */
	void updateAll();

	auto getColliding() -> std::vector<std::weak_ptr<lif::Collider>>& {
		return collidingEntities;
	}

	auto getColliding() const -> const std::vector<std::weak_ptr<lif::Collider>>& {
		return collidingEntities;
	}

	/** @return all colliders intersecting `rect`.
	 *  NOTE: these pointers are only guaranteed to be valid until the next call to updateAll(), so
	 *  the caller should *not* retain them.
	 */
	auto getCollidersIntersecting(const sf::FloatRect& rect) const -> std::vector<lif::Collider*>;

	/** @return all entities whose tile is `tile` */
	auto getEntitiesAtTile(const sf::Vector2i& tile) const -> std::vector<std::weak_ptr<lif::Entity>>;

	inline void mtxLock() const {
#ifdef MULTITHREADED
		mutex.lock();
#endif
	}
	inline void mtxUnlock() const {
#ifdef MULTITHREADED
		mutex.unlock();
#endif
	}
};

///// Implementation /////

template<typename T>
lif::Entity* EntityGroup::add(std::shared_ptr<T> entity) {
	entity->init();
	entities.emplace_back(entity);
	return _putInAux(entities.back().get());
}

template<typename T>
size_t EntityGroup::size() const {
	return std::count_if(entities.begin(), entities.end(), [] (const auto& e) {
		return dynamic_cast<const T*>(e.get()) != nullptr;
	});
}

} // end namespace lif
