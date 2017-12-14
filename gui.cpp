#include "gui.hpp"

#include "mouse.hpp"

#include <iostream>

dialog::dialog(sf::Vector2f position_, std::list<std::string> options_)
		:	position(position_){
	
	sf::RectangleShape area(sf::Vector2f(50, 17));
	sf::Vector2f curr_area_position = position_;
	for(auto& entry : options_){
		area.setPosition(curr_area_position);
		this->components.push_front(dialog_component(entry, area));
		curr_area_position.y += 17;
	}
}

void dialog::draw(sf::RenderWindow &window) const{
	for(auto& entry : this->components){
		entry.draw(window);
	}
}

void dialog::update(sf::Vector2f mouse_world_position){
	for(auto& entry : this->components){
		entry.update(mouse_world_position);
	}
}

bool dialog::check(sf::Vector2f position_) const{
	
}

std::string dialog::get(sf::Vector2f position_) const{
	
}

dialog_component::dialog_component(std::string text_, sf::RectangleShape area_)
		:	area(area_),
			text(text_){
	
}

std::string dialog_component::get_text() const{
	
}

bool dialog_component::check(sf::Vector2f position_) const{
	
}

void dialog_component::draw(sf::RenderWindow &window) const{
	sf::Font font;
	font.loadFromFile("arial.ttf");
	
	sf::Text label(this->text, font);
	label.setPosition(this->area.getPosition());
	label.setCharacterSize(15);
	label.setStyle(sf::Text::Bold);
	label.setColor(sf::Color::Red);
	
	window.draw(this->area);
	window.draw(label);
}

void dialog_component::update(sf::Vector2f mouse_world_position){
	if(mouse_check(mouse_world_position, this->area)){
		this->area.setFillColor(sf::Color::Black);
	} else {
		this->area.setFillColor(sf::Color::White);
	}
}