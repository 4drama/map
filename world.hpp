#ifndef WORLD__
#define WORLD__

#include <SFML/Graphics.hpp>

#include "keyboard.hpp"
#include "cache.hpp"
#include "file_cache.hpp"
#include "surface.hpp"
#include "map_collection.hpp"
#include "object_collection.hpp"
#include "interact_gui.hpp"

#include <memory>

#include "gui.hpp"

class World{
public:
	World();
	void Cicle();
	
private:
	sf::VideoMode resolution;
	sf::RenderWindow window;
	sf::View view;
	
	sf::Clock clock;
	float time;
	
	ControlFunction keyboard;
	
	std::shared_ptr<Cache> cache;
	std::shared_ptr<file_cache<surface> > cache_surface_ptr;
	Map_collection map;
	
	sf::Texture frame_texture;
	sf::Sprite frame_sprite;
	
	object_collection objects;
	std::weak_ptr<object> player;
	
	std::shared_ptr<interact_gui> three;
//	std::shared_ptr<dialog> two;
//	dialog one;
};

#endif