#include "mouse.hpp"


sf::Vector2f mouse_world_position(sf::RenderWindow &window, sf::View &view){
	sf::Vector2u resolution = window.getSize();
	
	sf::Vector2i mouse_position 	= sf::Mouse::getPosition(window);
	sf::Vector2f centre_position	= view.getCenter();
	sf::Vector2f offset(	(resolution.x / 2) * 0.75,
							(resolution.y / 2) * 0.75	);
	
	sf::Vector2f world_position
			(	centre_position.x - offset.x + (mouse_position.x * 0.75),
				centre_position.y - offset.y + (mouse_position.y * 0.75));
	return world_position;
}

bool mouse_check(sf::Vector2f position, sf::RectangleShape area){
	sf::Vector2f size = area.getSize();
	
	sf::Vector2f start_position = area.getPosition();
	sf::Vector2f end_position(start_position.x + size.x, start_position.y + size.y);
	if(	((position.x > start_position.x) && (position.x < end_position.x))	&&
		((position.y > start_position.y) && (position.y < end_position.y))	)
		return true;
	else
		return false;
}