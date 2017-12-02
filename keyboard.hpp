#ifndef KEYBOARD__
#define KEYBOARD__

#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <functional>


class ControlFunction{
private:
	using Key_list_type = 		std::vector 	<	std::pair<	sf::Keyboard::Key,
													std::function<void(float)	> > >;
										
	using Button_list_type = 	std::vector 	<	std::pair<	sf::Mouse::Button,
													std::function<void(float)	> > >;
	
	Key_list_type		keys;
	Button_list_type	buttons;
public:
	KeyboardFunction(){
		
	}
	
	void add(sf::Keyboard::Key key, std::function<void(float)> func){
		keys.push_back(std::make_pair(key, func));
	}
	
	void add(sf::Mouse::Button button, std::function<void(float)> func){
		buttons.push_back(std::make_pair(button, func));
	}
	
	void update(float t){
		
		for(auto i : keys){
			if(sf::Keyboard::isKeyPressed(i.first))
				i.second(t);
		}


		for(auto i : buttons){
			if(sf::Mouse::isButtonPressed(i.first))
				i.second(t);			
		}
	}
};

#endif