#ifndef CACHE__
#define CACHE__

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <functional>

#include <iostream>

class Cache : public std::enable_shared_from_this<Cache>{
	using Texture_weak_ptr = std::weak_ptr<sf::Texture>;
	using Texture_strong_ptr = std::shared_ptr<sf::Texture>;	
public:
	Texture_strong_ptr load(const std::string &filename){
		
		Texture_strong_ptr current;
		
		auto it = entries.find(filename);		
		if(it == entries.end()) {
			auto deleter = [self = shared_from_this(), filename](sf::Texture *texture){
				std::cerr << filename;
				delete texture;
				self->entries.erase(filename);
				std::cerr << " delete." << std::endl;
			};
			
			current = Texture_strong_ptr(new sf::Texture(), deleter);
			current->loadFromFile(filename);
			entries[filename] = current;
			std::cerr << filename << " load." << std::endl;
		} else
			current = entries[filename].lock();
		
		return current;
	}
	
	std::function<sf::Texture()> loadf(const std::string &filename){
		Texture_strong_ptr current = this->load(filename);
		return [current](){return *current;};
	}
	
private:
	std::map<std::string, Texture_weak_ptr> entries;
};

#endif