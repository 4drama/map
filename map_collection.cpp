#include "map_collection.hpp"

#include <iostream>
#include <cassert>

std::shared_ptr<surface> get_surface(sf::Vector2f new_position, Map_collection &map){
	std::list<std::vector<Tile>::iterator> current_tiles;
	current_tiles = map.get_tile(new_position);
	assert(!current_tiles.empty());
	std::shared_ptr<surface> next_surface;
	next_surface = current_tiles.front()->get_surface(new_position);
	return next_surface;
}

Map_collection::Map_collection(const std::string& filename, std::shared_ptr<Cache>& conteiner,
		std::shared_ptr<file_cache<surface> > cache_surface_ptr){
	std::fstream complete_map_file(filename, std::ios::in);
	std::string command;
	
	while(command != "end_final"){
		complete_map_file >> command;
		if(command == "init_block"){
			this->init_block(complete_map_file, conteiner, cache_surface_ptr);
		}
	}
	
}

void Map_collection::draw(sf::RenderWindow &window, sf::View &view){
	for (auto& current_map : complete_map){
		current_map.render(window, view);
	}
}

std::list<std::vector<Tile>::iterator> Map_collection::get_tile(sf::Vector2f point){
	std::list<std::vector<Tile>::iterator> tile_list;
	if(grab_chank.size() > 20){
		grab_chank.clear();
		std::cerr << "grab_chank.clear();\n";
	}
	
	for (auto& current_map : complete_map){
		if(current_map.check_position(point)){
			
			std::shared_ptr<map_chank> chank = current_map.get();
			grab_chank.insert(chank);
			assert(chank != nullptr);
			std::list<std::vector<Tile>::iterator> curr_list = chank->get_tile(point);
			tile_list.insert(tile_list.end(), curr_list.begin(), curr_list.end());
		}
	}
	
	return tile_list;
}

std::list<std::shared_ptr<object> > Map_collection::get_objects(sf::Vector2f point){
	std::list<std::shared_ptr<object> > result;
	
	std::list<std::vector<Tile>::iterator> tiles = this->get_tile(point);

	tiles.merge(this->get_tile
			(sf::Vector2f{(float)(point.x - 20), (float)(point.y - 20)}));
	tiles.merge(this->get_tile
			(sf::Vector2f{(float)(point.x + 20), (float)(point.y - 20)}));
	tiles.merge(this->get_tile
			(sf::Vector2f{(float)(point.x - 20), (float)(point.y + 20)}));
	tiles.merge(this->get_tile
			(sf::Vector2f{(float)(point.x + 20), (float)(point.y + 20)}));
	tiles.unique();
	
	for (auto& current_tile : tiles){
		std::list<std::shared_ptr<object> > objects = current_tile->get_objects();
		result.merge(objects);
	}
	
	result.unique();
	
	std::shared_ptr<interact_handler> interact;
	for (auto& object : result){
		interact = object->get_interact();
		
		geometry::Point position = interact->get_position(*object);
		double max_range = interact->get_radius(*object);
		
		double curr_range = geometry::Range(position, geometry::Point{point.x, point.y});
		if(curr_range > max_range){
			result.remove(object);
		}
	}
	
	std::cerr << "objects in " << point.x << ',' << point.y << " = " << result.size() << std::endl;
	
	return result;
}

void Map_collection::init_block(std::fstream &complete_map_file, std::shared_ptr<Cache>& conteiner,
		std::shared_ptr<file_cache<surface> > cache_surface_ptr){
	std::string command;
	std::string filename;
	int x_position, y_position;
	int width, height;
	
	while(command != "end_block"){
		complete_map_file >> command;
		if(command == "add"){
			complete_map_file >> filename >> x_position >> y_position >> width >> height;
			complete_map.push_back(
					auto_loading_map_chank(	filename, conteiner,
											x_position, y_position, 
											width, height, cache_surface_ptr));
		}
	}
}