#ifndef FILE_CACHE__
#define FILE_CACHE__

#include <memory>

template<class T>
class file_cache : public std::enable_shared_from_this<file_cache<T> >{
	using value_type_weak_ptr = std::weak_ptr<T>;
	using value_type_strong_ptr = std::shared_ptr<T>;
public:

	value_type_strong_ptr load(const std::string &filename){
		value_type_strong_ptr current;

		auto it = entries.find(filename);

		if(it == entries.end()) {
			auto deleter = [self = this->shared_from_this(), filename](T *value){
				delete value;
				self->entries.erase(filename);
			};
			current = value_type_strong_ptr(new T(filename), deleter);
			entries[filename] = current;
		} else
			current = entries[filename].lock();

		return current;
	}
	
private:
	std::map<std::string, value_type_weak_ptr> entries;
};

#endif