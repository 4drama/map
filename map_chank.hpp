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

class map_chank;
class auto_loading_map_chank;

//=====================map_chank declaration=========================
class map_chank{
public:
	map_chank(		const std::string& filename, std::shared_ptr<Cache>& conteiner,
					int x_position, int y_position, 
					std::shared_ptr<file_cache<surface> > cache_surface_ptr);
	void render(	sf::RenderWindow &window, sf::View &view);
	
	std::list<std::vector<Tile>::iterator> get_tile(const sf::Vector2f point);
	
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

//==================auto_loading_map_chank declaration===============
class auto_loading_map_chank{
public:
	auto_loading_map_chank(		const std::string& filename_, std::shared_ptr<Cache>& conteiner_,
								int x_position, int y_position, int width, int height,
								std::shared_ptr<file_cache<surface> > cache_surface_ptr);	
	void render(sf::RenderWindow &window, sf::View &view);	
	bool check_position(const sf::Vector2f point) const;	
	std::shared_ptr<map_chank> get();
	
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

#endif