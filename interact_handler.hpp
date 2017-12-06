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
	
	void init(Map_collection *map_){
		this->map = map_;
	}
	
private:
	
	bool check(object &self, geometry::Point new_position) override{
		if(this->get_surface(self) == nullptr)
			this->set_surface(self, this->update_surface(
							sf::Vector2f{(float)new_position.x, (float)new_position.y}));
		
		std::shared_ptr<surface> next_surface;
		next_surface = this->update_surface(
							sf::Vector2f{(float)new_position.x, (float)new_position.y});
		
		std::list<std::vector<Tile>::iterator> tiles = this->near_tiles(self);
		
		if(	(next_surface->get_type() == this->get_surface_type(self))	&&
			(!this->collision(tiles, self, new_position))){
				this->set_surface(self, next_surface);
				for (auto& current_tile : tiles){
					current_tile->remove_object(self);
				}				
				this->set_position(self, new_position);
				std::list<std::vector<Tile>::iterator> current_tiles;
				current_tiles = this->map->get_tile(
						sf::Vector2f{(float)new_position.x, (float)new_position.y});
				current_tiles.front()->add_object(self);
				return true;
		} else 
			return false;
	}
	
	bool collision(	std::list<std::vector<Tile>::iterator> &tiles, 
					object &self, 
					geometry::Point new_position){
		std::list<std::shared_ptr<object> > objects_near;
		for (auto& current_tile : tiles){
			objects_near.merge(current_tile->get_objects());
		}
		objects_near.unique();
		objects_near.remove_if([need = self.shared_from_this()](std::shared_ptr<object> current){
				return (current == need) ? true : false;
			});
		
		for (auto& obj : objects_near){
			double new_range = geometry::Range(new_position, this->get_position(*obj));
			double current_range = geometry::Range(this->get_position(self), this->get_position(*obj));
			double max_range = this->get_radius(self) + this->get_radius(*obj);
			if(new_range <= max_range && new_range <= current_range)
				return true;
		}
		
		return false;
	}
	
	std::list<std::vector<Tile>::iterator> near_tiles(object &self){
		std::list<std::vector<Tile>::iterator> all_near_tiles;
		geometry::Point position = this->get_position(self);
		float radius = this->get_radius(self);
		
		all_near_tiles.merge(map->get_tile
				(sf::Vector2f{(float)(position.x - radius), (float)(position.y - radius)}));
		all_near_tiles.merge(map->get_tile
				(sf::Vector2f{(float)(position.x + radius), (float)(position.y - radius)}));
		all_near_tiles.merge(map->get_tile
				(sf::Vector2f{(float)(position.x - radius), (float)(position.y + radius)}));
		all_near_tiles.merge(map->get_tile
				(sf::Vector2f{(float)(position.x + radius), (float)(position.y + radius)}));
		
		all_near_tiles.unique();
		
		return all_near_tiles;
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