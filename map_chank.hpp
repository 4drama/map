#ifndef MAP_CHANK__
#define MAP_CHANK__

#include <SFML/Graphics.hpp>

#include "tiles.hpp"
#include "cache.hpp"

#include <fstream>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <list>

#include <iostream>
#include <cmath>

#include <cassert>

#define MAX_WIDTH_VIEW_DEFFERENCE	1000
#define MAX_HEIGHT_VIEW_DEFFERENCE	1000

class map_chank;
class auto_loading_map_chank;

//=====================map_chank declaration=========================
class map_chank{
public:
	map_chank(		const std::string& filename, std::shared_ptr<Cache>& conteiner,
					int x_position, int y_position, 
					std::shared_ptr<file_cache<surface> > cache_surface_ptr);
	void render(	sf::RenderWindow &window, sf::View &view);
	
	std::list<std::vector<Tile>::iterator> get_tile(const sf::Vector2f point){
		std::list<std::vector<Tile>::iterator> tiles_list;
		
		for (auto it = tiles.begin(); it != tiles.end() ; it++){
			if(it->check_position(point)){
				tiles_list.push_back(it);
			}
		}
		
		
		assert(!tiles_list.empty());
		return tiles_list;
	}
	
private:	
	std::vector<Tile> tiles;
	
	void fileparse(		const std::string &filename,
						std::shared_ptr<Cache> &conteiner,
						int x_position, int y_position,
						std::shared_ptr<file_cache<surface> > cache_surface_ptr);	
	void tiles_block(	std::fstream &map_file, 
						std::map<std::string, std::shared_ptr<Tile_base> > &base,
						int width, int height,
						int x_position_start, int y_position_start);	
	void init_block(	std::fstream &map_file, 
						std::map<std::string, std::shared_ptr<Tile_base> > &base,
						std::shared_ptr<Cache> &conteiner,
						int &width, int &height,
						int &x_position, int &y_position,
						std::shared_ptr<file_cache<surface> > cache_surface_ptr);
};
//-------------------------------------------------------------------

//=====================map_chank implementation======================
map_chank::map_chank(		const std::string &filename_, 
							std::shared_ptr<Cache> &conteiner_,
							int x_position, int y_position,
							std::shared_ptr<file_cache<surface> > cache_surface_ptr){
	fileparse(filename_, conteiner_, x_position, y_position, cache_surface_ptr);
}

void map_chank::render(sf::RenderWindow &window, sf::View &view){
	for (auto& current_tile : tiles){
		current_tile.draw(window, view);
	}
}

void map_chank::fileparse(	const std::string &filename,
							std::shared_ptr<Cache> &conteiner,
							int x_position, int y_position,
							std::shared_ptr<file_cache<surface> > cache_surface_ptr){
		
	std::fstream map_file(filename, std::ios::in);	
	std::string command;
	std::map<std::string, std::shared_ptr<Tile_base> > base;
	int width = 50, height = 50;
//	int x_position = 0, y_position = 0;
	
	while(command != "end_final"){
		map_file >> command;
		if(command == "init_block"){			
			this->init_block(	map_file, base, conteiner, width, height, 
								x_position, y_position, cache_surface_ptr);
		} else if(command == "tiles_block"){
			this->tiles_block(map_file, base, width, height, x_position, y_position);
		}
	}
}

void map_chank::tiles_block(	std::fstream &map_file, 
								std::map<std::string, std::shared_ptr<Tile_base> > &base,
								int width, int height,
								int x_position_start, int y_position_start){
	
	std::string command;
	map_file >> command;
	
	int 	x_position = x_position_start,
			y_position = y_position_start;
	
	do{	
		if(command == "end_lines"){
			y_position += height;
			x_position = x_position_start;
		} else {
			if(base[command] != nullptr)
				this->tiles.push_back(base[command]->create_tile(x_position, y_position));
			x_position += width;
		}
		
		map_file >> command;
	} while (command != "end_block");
}

void map_chank::init_block(	std::fstream &map_file, 
							std::map<std::string, std::shared_ptr<Tile_base> > &base,
							std::shared_ptr<Cache> &conteiner,
							int &width, int &height,
							int &x_position, int &y_position,
							std::shared_ptr<file_cache<surface> > cache_surface_ptr){
	
	std::string command;
	
	std::string name;
	std::string tile_path;
	std::string surface_path;
	
	while(command != "end_block"){
		map_file >> command;
		if(command == "add"){
			map_file >> name >> tile_path >> surface_path;
			
			std::shared_ptr<Tile_base> current(
					new Tile_base(	tile_path, surface_path, width, height,
									conteiner, cache_surface_ptr));
							
			base[name] = current;
		} else if(command == "size"){
			map_file >> width >> height;
		} else if(command == "position"){
			map_file >> x_position >> y_position;
		}
	}
}
//-------------------------------------------------------------------

//==================auto_loading_map_chank declaration===============
class auto_loading_map_chank{
public:
	auto_loading_map_chank(		const std::string& filename_, std::shared_ptr<Cache>& conteiner_,
								int x_position, int y_position, int width, int height,
								std::shared_ptr<file_cache<surface> > cache_surface_ptr);
	
	void render(sf::RenderWindow &window, sf::View &view);
	
	bool check_position(const sf::Vector2f point) const{
		if(	((point.x >= area.left) && (point.x <= (area.left + area.width)))	&&
			((point.y >= area.top) && (point.y <= (area.top + area.height)))	){
			
			return true;
		}
		else {
			return false;
		}
	}
	
	std::shared_ptr<map_chank> get(){
		if(this->chank == nullptr){
			std::shared_ptr<map_chank> result;
			if(!support_ptr.use_count()){
				result = std::make_shared<map_chank>(	filename, conteiner, area.left, 
														area.top, cache_surface_ptr);
				support_ptr = result;
			} else {
				result = support_ptr.lock();
			}

			return result;
		}
		
		return this->chank;
	}
	
private:
	enum State{
		outView,
		inView
	};

	std::shared_ptr<map_chank> 				chank;
	std::weak_ptr<map_chank>				support_ptr;
	sf::Rect<int>							area;
	std::shared_ptr<Cache>					conteiner;
	std::shared_ptr<file_cache<surface> > 	cache_surface_ptr;
	std::string								filename;
	State									state;
	
	sf::Vector2f							max_difference;
	
	bool check_view(const sf::View &view) const;
};
//-------------------------------------------------------------------

//===============auto_loading_map_chank implementation===============
auto_loading_map_chank::auto_loading_map_chank(
				const std::string& filename_, std::shared_ptr<Cache>& conteiner_,
				int x_position, int y_position, int width, int height,
				std::shared_ptr<file_cache<surface> > cache_surface_ptr_)
		:	conteiner(conteiner_), filename(filename_), state(State::outView),
			cache_surface_ptr(cache_surface_ptr_){
	area.left = x_position;
	area.top = y_position;
	area.width = width;
	area.height = height;
	max_difference = sf::Vector2f(MAX_WIDTH_VIEW_DEFFERENCE, MAX_HEIGHT_VIEW_DEFFERENCE);
}

void auto_loading_map_chank::render(	sf::RenderWindow &window, sf::View &view){
	if(state == State::outView){
		if(check_view(view)){
			if(!support_ptr.use_count()){
				chank = std::make_shared<map_chank>(	filename, conteiner, area.left, 
														area.top, cache_surface_ptr);
			} else 
				chank = support_ptr.lock();
				
			state = State::inView;
			this->render(window, view);
			std::cerr << filename << " inView." << std::endl;
		}
	} else if(state == State::inView){
		if(check_view(view)){
			chank->render(window, view);
		} else {
			chank = nullptr;
			state = State::outView;
			std::cerr << filename << " outView." << std::endl;
		}
	}
}

bool auto_loading_map_chank::check_view(const sf::View &view) const{
	sf::Vector2f view_centre(view.getCenter());
	sf::Vector2f chank_centre(area.left+(area.width/2), area.top+(area.height/2));
	
	sf::Vector2f current_difference(	std::abs(view_centre.x - chank_centre.x),
										std::abs(view_centre.y - chank_centre.y)	);
	
	return (	current_difference.x > max_difference.x ||
				current_difference.y > max_difference.y		)
				? false : true;
}
//-------------------------------------------------------------------

#undef MAX_WIDTH_VIEW_DEFFERENCE
#undef MAX_HEIGHT_VIEW_DEFFERENCE

#endif