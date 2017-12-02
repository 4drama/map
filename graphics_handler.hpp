#ifndef GRAPHICS_HANDLER__
#define GRAPHICS_HANDLER__

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "geometry.hpp"

class object;

sf::Vector2f centre_convert(geometry::Point point, float radius){
	return sf::Vector2f{(float)point.x - radius, (float)point.y - radius};
}

class window_graphics : public graphics_handler{
public:
	window_graphics() = default;
	
	void draw(object &self) override{
		std::shared_ptr<interact_handler> interact = self.get_interact();
		
		sf::CircleShape zone;
		zone.setRadius(interact->get_radius(self));
		zone.setFillColor(sf::Color::Magenta);
		zone.setPosition(centre_convert(interact->get_position(self), interact->get_radius(self)));
		window->draw(zone);
	}
	
	void init(sf::RenderWindow *window_){
		window = window_;
	}
	
private:
	sf::RenderWindow *window;
};

#endif