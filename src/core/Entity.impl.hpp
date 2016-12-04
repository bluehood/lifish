/// Entity template methods implementation

template<class T>
T* Entity::addComponent(T* comp) {
	if (T::requiredUnique() && get<T>() != nullptr)
		throw std::logic_error("Two components of the same type were added to this Entity!");
	components.push_back(std::unique_ptr<Game::Component>(comp));
	return comp;
} 

template<class T>
bool Entity::is() const {
	return dynamic_cast<const T*>(this);
}

template<class T>
bool Entity::isExactly() const {
	return strcmp(T::_ENTITY_ID, getEntityID()) == 0;
}

template<class T>
T* Entity::get() const {
	for (auto& comp : components) {
		Component *ptr = comp.get();
		if (ptr->is<T>())
			return static_cast<T*>(ptr);
	}
	return nullptr;
}

template<class T>
std::vector<T*> Entity::getAll() const {
	std::vector<T*> all;
	for (auto& comp : components) {
		Component *ptr = comp.get();
		T* derived = nullptr;
		if (ptr->is<T>())
			all.push_back(static_cast<T*>(ptr));
	}
	return all;
}

template<class T>
std::vector<T*> Entity::getAllRecursive() const {
	std::vector<T*> all;
	for (auto& comp : components) {
		Component *ptr = comp.get();
		if (ptr->is<T>())
			all.push_back(static_cast<T*>(ptr));
		auto sub = ptr->getAllRecursive<T>();
		all.insert(all.end(), sub.begin(), sub.end());
	}
	return all;
}

template<class T>
std::shared_ptr<T> Entity::getShared() const {
	for (auto& comp : components) {
		Component *ptr = comp.get();
		if (ptr->is<T>())
			return std::static_pointer_cast<T>(comp);
	}
	return std::shared_ptr<T>();
}

template<class T>
std::vector<std::shared_ptr<T>> Entity::getAllRecursiveShared() const {
	std::vector<std::shared_ptr<T>> all;
	for (auto& comp : components) {
		Component *ptr = comp.get();
		if (ptr->is<T>())
			all.push_back(std::static_pointer_cast<T>(comp));
		auto sub = ptr->getAllRecursiveShared<T>();
		all.insert(all.end(), sub.begin(), sub.end());
	}
	return all;
}
