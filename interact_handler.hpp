#ifndef INTERACT_HANDLER__
#define INTERACT_HANDLER__

#include <SFML/Graphics.hpp>

#include "geometry.hpp"
#include "object.hpp"
#include "map_collection.hpp"
#include "surface.hpp"

#include <list>
#include <vector>
#include <cassert>

class object_interact	:	public interact_handler{
public:
	object_interact() = default;
	
	void update(object &self, float time) override{
		this->move(self, time);
	}

	void init(Map_collection *map_){
		this->map = map_;
	}
	
private:
	void move(object &self, float time){
		geometry::Point old_position = this->get_position(self);
		geometry::Point end_position = this->get_target(self);
		
		if(old_position != end_position){
			
			geometry::Point new_position;
			
			if(this->get_surface(self) == nullptr)
				this->set_surface(self, this->update_surface(
							sf::Vector2f{(float)old_position.x, (float)old_position.y}));
			
			float new_speed = this->get_speed(self) * this->get_surface(self)->get_modify();
			
			new_position = geometry::Shift(	old_position, end_position, time, new_speed);
			
			std::shared_ptr<surface> next_surface;
			next_surface = this->update_surface(
							sf::Vector2f{(float)new_position.x, (float)new_position.y});
			
			if(next_surface->get_type() == this->get_surface_type(self)){
				this->set_surface(self, next_surface);
				this->set_position(self, new_position);
			} else {
				this->set_target(self, old_position);
			}
			return;
		} else 
			return;
	}
	
	std::shared_ptr<surface> update_surface(sf::Vector2f new_position){
		std::list<std::vector<Tile>::iterator> current_tiles;
		current_tiles = this->map->get_tile(new_position);
		assert(!current_tiles.empty());
		std::shared_ptr<surface> next_surface;
		next_surface = current_tiles.front()->get_surface(new_position);
		return next_surface;
	}
	
	Map_collection *map;
};

#endif