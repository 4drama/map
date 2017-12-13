#include "graphics_handler.hpp"

#include <memory>

sf::Vector2f centre_convert(geometry::Point point, float radius){
	return sf::Vector2f{(float)point.x - radius, (float)point.y - radius};
}

void window_graphics::draw(object &self){
	std::shared_ptr<interact_handler> interact = self.get_interact();
	
	sf::CircleShape zone;
	zone.setRadius(interact->get_radius(self));
	zone.setFillColor(sf::Color::Magenta);
	zone.setPosition(centre_convert(interact->get_position(self), interact->get_radius(self)));
	window->draw(zone);
}

void window_graphics::init(sf::RenderWindow *window_){
	window = window_;
}