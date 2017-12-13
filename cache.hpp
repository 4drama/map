#ifndef CACHE__
#define CACHE__

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <functional>

class Cache : public std::enable_shared_from_this<Cache>{
	using Texture_weak_ptr = std::weak_ptr<sf::Texture>;
	using Texture_strong_ptr = std::shared_ptr<sf::Texture>;	
public:

	Texture_strong_ptr load(const std::string &filename);	
	std::function<sf::Texture()> loadf(const std::string &filename);
	
private:
	std::map<std::string, Texture_weak_ptr> entries;
};

#endif