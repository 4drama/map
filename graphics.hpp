#ifndef GRAPHICS__
#define GRAPHICS__

#include <SFML/Graphics.hpp>

#include <list>
#include <memory>
#include <utility>

class animation{
public:
	animation() = default;
//	animation(const animation& other);

	void init(sf::Texture *texture);
	void add_frame(sf::IntRect rect, sf::Vector2f pivot);
	
	sf::Vector2f get_pivot();
	void draw(sf::RenderWindow *window, sf::Vector2f position);
	void update(float time);
private:
	void next_frame();
	
	sf::Sprite current_sprite;

	std::list<std::pair<sf::IntRect, sf::Vector2f> > orientation_list;
	std::list<std::pair<sf::IntRect, sf::Vector2f> >::iterator current_frame;
};

class graphics{
public:
	sf::Texture* add_texture(std::string name, std::string path);
	void add_animation(std::string animation_name, animation &&new_animation);
	animation& get_animation(std::string name);
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, animation> animations;
};

class animation_manager{
public:
	void init(std::shared_ptr<graphics> graphics_ptr_, std::string start_animation);
	
	void set_animation(std::string name);
	animation& get_animation();
private:
	std::string current_animation;
	std::shared_ptr<graphics> graphics_ptr;
};

#endif