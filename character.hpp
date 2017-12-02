#ifndef CHARACTER__
#define CHARACTER__

#include <SFML/Graphics.hpp>

#include "geometry.hpp"
#include "surface.hpp"
#include "map_collection.hpp"

#include <cassert>

geometry::Point VectorToPoint(sf::Vector2f vec){
	return geometry::Point{vec.x, vec.y};	
}

sf::Vector2f PointToVector(geometry::Point p){
	return sf::Vector2f(p.x, p.y);
}

class character{
public:
	character(float x, float y)
			:	speed(0.05),
				target_position(sf::Vector2f(x, y)){
		zone.setRadius(5);
		zone.setPosition(target_position);
	}
	
	character() = default;
	
	void draw(sf::RenderWindow &window){
		window.draw(zone);
	}
	
	void update(float time, Map_collection &map){
		if(get_centre() != target_position){
			
			geometry::Point new_position;
			geometry::Point old_position = VectorToPoint(get_centre());
			geometry::Point end_position = VectorToPoint(target_position);
			
			if(current_surface == nullptr)
				this->current_surface = this->get_surface(target_position, map);
			
			float new_speed = speed * current_surface->get_modify();
			
		//	std::cerr << speed << '*' << current_surface->get_modify() << '=' << new_speed << std::endl;
			
			new_position = geometry::Shift(	old_position, end_position, time, new_speed);			
			
			std::shared_ptr<surface> next_surface;
			next_surface = this->get_surface(PointToVector(new_position), map);
			
			if(next_surface->get_type() == SURFACE_TYPE::SOLID){
				current_surface = next_surface;
				set_centre(PointToVector(new_position));
			} else {
				this->set_target(this->get_centre());
			}
			
		//	std::cerr << zone.getPoint(0).x << ' ' << zone.getPoint(0).y << std::endl;
		} else 
			return;
	}
	
	void set_target(float x, float y){
		this->set_target(sf::Vector2f(x, y));
	}
	
	void set_target(sf::Vector2f pos){
		target_position = pos;
	}
	
private:
	sf::Vector2f get_centre(){
		sf::Vector2f result = zone.getPosition();
		result.x += zone.getRadius();
		result.y += zone.getRadius();
		return result;
	}
	
	void set_centre(sf::Vector2f new_position){
		new_position.x -= zone.getRadius();
		new_position.y -= zone.getRadius();
		zone.setPosition(new_position);
	}
	
	std::shared_ptr<surface> get_surface(sf::Vector2f new_position, Map_collection &map){
		std::list<std::vector<Tile>::iterator> current_tiles;
		current_tiles = map.get_tile(new_position);
		assert(!current_tiles.empty());
		std::shared_ptr<surface> next_surface;
		next_surface = current_tiles.front()->get_surface(new_position);
		return next_surface;
	}

	sf::CircleShape zone;
	float speed;
	sf::Vector2f target_position;
	std::shared_ptr<surface> current_surface;
};

#endif