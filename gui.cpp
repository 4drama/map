#include "gui.hpp"

#include "mouse.hpp"

#include <iostream>

dialog::dialog(sf::Vector2f position_, std::list<std::string> options_)
		:	position(position_){
	
	sf::RectangleShape area(sf::Vector2f(50, 17));
	sf::Vector2f curr_area_position = position_;
	int size = 0;
	for(auto& entry : options_){
		size++;
		area.setPosition(curr_area_position);
		this->components.push_front(dialog_component(entry, area));
		curr_area_position.y += 17;
	}
	
	this->frame.setPosition(sf::Vector2f(position.x - 1, position.y - 2));
	this->frame.setSize(sf::Vector2f(52, (17 * size) + 4));
	this->frame.setFillColor(sf::Color::Black);
}

void dialog::draw(sf::RenderWindow &window) const{
	if(!this->components.empty()){
		window.draw(this->frame);
		for(auto& entry : this->components){
			entry.draw(window);
		}
	}
}

void dialog::update(sf::Vector2f mouse_world_position){
	if(this->check(mouse_world_position)){
		for(auto& entry : this->components){
			entry.update(mouse_world_position);
		}
	}
}

bool dialog::check(sf::Vector2f position_) const{
	return mouse_check(position_, this->frame);
}

std::string dialog::get(sf::Vector2f position_) const{
	std::string result;
	for(auto& entry : this->components){
		if(entry.check(position_)){
			result = entry.get_text();
			break;
		}
	}
	return result;
}

dialog_component::dialog_component(std::string text_, sf::RectangleShape area_)
		:	area(area_),
			text(text_),
			text_color(sf::Color::Black){
}

std::string dialog_component::get_text() const{
	return this->text;
}

bool dialog_component::check(sf::Vector2f position_) const{
	return mouse_check(position_, this->area);
}

void dialog_component::draw(sf::RenderWindow &window) const{
	sf::Font font2;
	font2.loadFromFile("arial.ttf");
	
	sf::Text label2(this->text, font2);
	sf::Vector2f text_position = area.getPosition();
	text_position.x += 4;
	label2.setPosition(text_position);
	label2.setCharacterSize(15);
	label2.setStyle(sf::Text::Bold);
	label2.setColor(this->text_color);
	
	window.draw(this->area);
	window.draw(label2);
}

void dialog_component::update(sf::Vector2f mouse_world_position){
	if(this->check(mouse_world_position)){
		this->area.setFillColor(sf::Color::Black);
		this->text_color = sf::Color::White;
	} else {
		this->area.setFillColor(sf::Color::White);
		this->text_color = sf::Color::Black;
	}
}