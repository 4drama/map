#include "keyboard.hpp"

void ControlFunction::add(sf::Keyboard::Key key, std::function<void(float)> func){
	keys.push_back(std::make_pair(key, func));
}

void ControlFunction::add(sf::Mouse::Button button, std::function<void(float)> func){
	buttons.push_back(std::make_pair(button, func));
}

void ControlFunction::update(float t){
	
	for(auto i : keys){
		if(sf::Keyboard::isKeyPressed(i.first))
			i.second(t);
	}


	for(auto i : buttons){
		if(sf::Mouse::isButtonPressed(i.first))
			i.second(t);			
	}
}