#ifndef MAP_COLLECTION__
#define MAP_COLLECTION__

#include <SFML/Graphics.hpp>

#include "map_chank.hpp"
#include "cache.hpp"

#include <string>
#include <list>

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

	Map_collection(	const std::string& filename, std::shared_ptr<Cache>& conteiner,
					std::shared_ptr<file_cache<surface> > cache_surface_ptr);	
	void draw(sf::RenderWindow &window, sf::View &view);
	std::list<std::vector<Tile>::iterator> get_tile(sf::Vector2f point);
	
	std::list<std::shared_ptr<object> > get_objects(sf::Vector2f point);
	
private:
	std::vector<auto_loading_map_chank> complete_map;
//	std::set<std::shared_ptr<map_chank>, shared_cmp<map_chank> > grab_chank;
	std::set<std::shared_ptr<map_chank> > grab_chank;
	
	void init_block(std::fstream &complete_map_file, std::shared_ptr<Cache>& conteiner,
			std::shared_ptr<file_cache<surface> > cache_surface_ptr);
};

#endif