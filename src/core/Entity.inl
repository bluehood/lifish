/// Entity template methods implementation

template<class T>
Entity::CompKey Entity::_compKey() const {
	return std::type_index(typeid(T));
}

template<class Alias, class Real>
void Entity::aliasComponent(const Real& comp) {
	if (_compKey<Alias>() == _compKey<Real>()) 
		throw std::logic_error(std::string("registering alias with same key: ") + typeid(Alias).name());
	auto comps = getAllShared<Real>();
	for (const auto& c : comps) {
		if (c.get() == &comp) {
			components[_compKey<Alias>()].push_back(c);
			std::cout << "registered " << typeid(Alias).name() << " -> " << c.get() << std::endl;
			return;
		}
	}
}

template<class T>
T* Entity::addComponent(T* comp) {
	if (T::requiredUnique() && get<T>() != nullptr)
		throw std::logic_error("Two components of the same type were added to this Entity!");
	components[_compKey<T>()].push_back(std::shared_ptr<lif::Component>(comp));
	return comp;
}

template<class T>
T* Entity::get() const {
	auto shared = getShared<T>();
	return shared ? shared.get() : nullptr;
}

template<class T>
std::vector<T*> Entity::getAll() const {
	std::vector<T*> all;
	auto comps = components.find(_compKey<T>());
	if (comps != components.end() && comps->second.size() == 0)
		for (auto comp : comps->second)
			all.push_back(static_cast<T*>(comp.get()));
	return all;
}

template<class T>
std::vector<T*> Entity::getAllRecursive() const {
	std::vector<T*> all;
	for (const auto& pair : components) {
		for (const auto& comp : pair.second) {
			Component *ptr = comp.get();
			T* derived = nullptr;
			if (ptr && (derived = dynamic_cast<T*>(ptr)))
				all.push_back(derived);
			auto sub = ptr->getAllRecursive<T>();
			all.insert(all.end(), sub.begin(), sub.end());
		}
	}
	return all;
}

template<class T>
std::shared_ptr<T> Entity::getShared() const {
	auto comps = components.find(_compKey<T>());
	std::cout << "getShared<" << typeid(T).name() << ">; found? " << (comps != components.end())  << std::endl;
	if (comps == components.end() || comps->second.size() == 0)
		return nullptr;
	return std::static_pointer_cast<T>(comps->second[0]);
}

template<class T>
std::vector<std::shared_ptr<T>> Entity::getAllShared() const {
	std::vector<std::shared_ptr<T>> all;
	auto comps = components.find(_compKey<T>());
	if (comps != components.end())
		for (auto comp : comps->second)
			all.push_back(std::static_pointer_cast<T>(comp));
	return all;
}

template<class T>
std::vector<std::shared_ptr<T>> Entity::getAllRecursiveShared() const {
	std::vector<std::shared_ptr<T>> all;
	for (const auto& pair : components) {
		for (const auto& comp : pair.second) {
			Component *ptr = comp.get();
			if (ptr && dynamic_cast<T*>(ptr) != nullptr)
				all.push_back(std::static_pointer_cast<T>(comp));
			const auto sub = ptr->getAllRecursiveShared<T>();
			all.insert(all.end(), sub.begin(), sub.end());
		}
	}
	return all;
}
