#ifndef MOUSE__
#define MOUSE__

#include <SFML/Graphics.hpp>

sf::Vector2f mouse_world_position(sf::RenderWindow &window, sf::View &view);
bool mouse_check(sf::Vector2f position, sf::RectangleShape area);
#endif