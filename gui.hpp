#ifndef GUI__
#define GUI__

#include <SFML/Graphics.hpp>

#include <list>
#include <string>

class dialog_component;

class dialog{
public:
	dialog(sf::Vector2f position_, std::list<std::string> options_);
	
	void draw(sf::RenderWindow &window) const;
	void update(sf::Vector2f mouse_world_position);
	
	bool check(sf::Vector2f position_) const;
	std::string get(sf::Vector2f position_) const;
	
private:
	sf::Vector2f position;
	std::list<dialog_component> components;
};

class dialog_component{
public:
	dialog_component(std::string text_, sf::RectangleShape area_);
	std::string get_text() const;
	bool check(sf::Vector2f position_) const;
	void draw(sf::RenderWindow &window) const;
	void update(sf::Vector2f mouse_world_position);
	
private:
	sf::RectangleShape area;
	std::string text;
};

#endif