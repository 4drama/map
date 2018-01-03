#include "graphics_handler.hpp"

#include <memory>

#include <cassert>

sf::Vector2f centre_convert(geometry::Point point, float radius){
	return sf::Vector2f{(float)point.x - radius, (float)point.y - radius};
}
namespace{
	sf::Vector2f convert(geometry::Point point){
		return sf::Vector2f{(float)point.x, (float)point.y};
	}
	
	sf::Vector2f pivot_correct(geometry::Point point, sf::Vector2f pivot){
		return sf::Vector2f{(float)point.x - pivot.x, (float)point.y - pivot.y};
	}
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



void animation_graphics::draw(object &self){
	std::shared_ptr<interact_handler> interact = self.get_interact();
	std::shared_ptr<animation_manager> animations_ptr = self.get_animation_manager();
	sf::Vector2f position;
	sf::Vector2f pivot = animations_ptr->get_animation().get_pivot();
	position = pivot_correct(interact->get_position(self), pivot);;
	
	assert(animations_ptr != nullptr);
	animations_ptr->get_animation().draw(this->window, position);
	
	sf::CircleShape zone;
	zone.setRadius(interact->get_radius(self));
	zone.setFillColor(sf::Color::Magenta);
	zone.setPosition(centre_convert(interact->get_position(self), interact->get_radius(self)));
	window->draw(zone);
}

void animation_graphics::init(sf::RenderWindow *window_){
	window = window_;
}