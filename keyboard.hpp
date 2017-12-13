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
	
	void add(sf::Keyboard::Key key, std::function<void(float)> func);
	
	void add(sf::Mouse::Button button, std::function<void(float)> func);
	
	void update(float t);
};

#endif