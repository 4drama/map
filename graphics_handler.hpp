#ifndef GRAPHICS_HANDLER__
#define GRAPHICS_HANDLER__

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "geometry.hpp"

class object;

sf::Vector2f centre_convert(geometry::Point point, float radius);

class window_graphics : public graphics_handler{
public:

	window_graphics() = default;	
	void draw(object &self) override;	
	void init(sf::RenderWindow *window_);
	
private:
	sf::RenderWindow *window;
};

#endif