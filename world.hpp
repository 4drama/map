#ifndef WORLD__
#define WORLD__

#include <SFML/Graphics.hpp>

#include "services.hpp"

#include <memory>

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
	
	services game;

	sf::Texture frame_texture;
	sf::Sprite frame_sprite;

//	std::weak_ptr<object> player;
	std::shared_ptr<controlled_object> player;
};

#endif