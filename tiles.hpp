#ifndef TILES__
#define TILES__

#include <SFML/Graphics.hpp>

#include "cache.hpp"
#include "surface.hpp"
#include "object.hpp"

#include <memory>
#include <list>

class Tile_base;

class Tile{
	friend class Tile_base;
public:
	void draw(sf::RenderWindow &window, sf::View &view);
	
	bool check_position(const sf::Vector2f point);
	
	void add_object(object &obj);
	
	void remove_object(object &obj);
	
	std::list<std::shared_ptr<object> > get_objects() const;
	
/*	void test(){
		texture = nullptr;
		sprite.setTexture(*texture);
	}*/
	
	std::shared_ptr<surface> get_surface(sf::Vector2f point);
	
private:
	enum State{
		outView,
		inView
	};
	
	State state;
	sf::Sprite sprite;
	std::shared_ptr<Tile_base> base;
	std::shared_ptr<sf::Texture> texture;
	std::list<std::weak_ptr<object> > objects;
	
	Tile(int x_pos_, int y_pos_, std::shared_ptr<Tile_base> base_);
	bool view_check(sf::View &view);	
};

class Tile_base : public std::enable_shared_from_this<Tile_base>{
	friend class Tile;
public:
	Tile_base(	const std::string &filename_, const std::string &surface_path,
				int x_size_, int y_size_,
				std::shared_ptr<Cache> &conteiner_,
				std::shared_ptr<file_cache<surface> > cache_surface_ptr);
	
	Tile create_tile(int x_position, int y_position);
				
private:
	std::shared_ptr<Cache> conteiner;
	surface_area area;
	
	std::string filename;
	int x_size, y_size;
	
};

#endif