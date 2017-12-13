#ifndef INTERACT_HANDLER__
#define INTERACT_HANDLER__

#include <SFML/Graphics.hpp>

#include "geometry.hpp"
#include "object.hpp"
#include "map_collection.hpp"
#include "surface.hpp"

#include <list>
#include <vector>

class object_interact	:	public interact_handler{
public:
	object_interact() = default;
	
	void init(Map_collection *map_);
	
private:
	
	bool check(object &self, geometry::Point target_position, double radius) override;	
	bool walk(object &self, geometry::Point new_position) override;	
	bool collision(	std::list<std::vector<Tile>::iterator> &tiles, 
					object &self, 
					geometry::Point new_position);	
	std::list<std::vector<Tile>::iterator> near_tiles(object &self);	
	std::shared_ptr<surface> update_surface(sf::Vector2f new_position);
	
	double interact_range = 5;
	Map_collection *map;
};

#endif