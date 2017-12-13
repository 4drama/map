#include "cache.hpp"

#include <iostream>

Cache::Texture_strong_ptr Cache::load(const std::string &filename){
	
	Cache::Texture_strong_ptr current;
	
	auto it = entries.find(filename);		
	if(it == entries.end()) {
		auto deleter = [self = shared_from_this(), filename](sf::Texture *texture){
			std::cerr << filename;
			delete texture;
			self->entries.erase(filename);
			std::cerr << " delete." << std::endl;
		};
		
		current = Cache::Texture_strong_ptr(new sf::Texture(), deleter);
		current->loadFromFile(filename);
		entries[filename] = current;
		std::cerr << filename << " load." << std::endl;
	} else
		current = entries[filename].lock();
	
	return current;
}

std::function<sf::Texture()> Cache::loadf(const std::string &filename){
	Cache::Texture_strong_ptr current = this->load(filename);
	return [current](){return *current;};
}