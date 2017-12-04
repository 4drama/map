#ifndef MAP_COLLECTION__
#define MAP_COLLECTION__

#include <SFML/Graphics.hpp>

#include "map_chank.hpp"
#include "cache.hpp"

#include <string>
#include <list>
#include <iostream>
#include <cassert>

#include <set>
/*
template<class T>
class shared_cmp{
public:
	constexpr bool operator()( const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs ) const{
	//	std::cerr << (lhs.get() == rhs.get()) << '\n';
		return (lhs.get() != rhs.get()) ? true : false;
	}
};*/
class Map_collection;

std::shared_ptr<surface> get_surface(sf::Vector2f new_position, Map_collection &map);

class Map_collection{
public:
	Map_collection() = default;

	Map_collection(const std::string& filename, std::shared_ptr<Cache>& conteiner,
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
	
	void draw(sf::RenderWindow &window, sf::View &view){
		for (auto& current_map : complete_map){
			current_map.render(window, view);
		}
	}
	
	std::list<std::vector<Tile>::iterator> get_tile(sf::Vector2f point){
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
	};
	
private:
	std::vector<auto_loading_map_chank> complete_map;
//	std::set<std::shared_ptr<map_chank>, shared_cmp<map_chank> > grab_chank;
	std::set<std::shared_ptr<map_chank> > grab_chank;
	
	void init_block(std::fstream &complete_map_file, std::shared_ptr<Cache>& conteiner,
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
};

std::shared_ptr<surface> get_surface(sf::Vector2f new_position, Map_collection &map){
	std::list<std::vector<Tile>::iterator> current_tiles;
	current_tiles = map.get_tile(new_position);
	assert(!current_tiles.empty());
	std::shared_ptr<surface> next_surface;
	next_surface = current_tiles.front()->get_surface(new_position);
	return next_surface;
}

#endif