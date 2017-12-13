#ifndef SURFACE__
#define SURFACE__

#include "file_cache.hpp"

#include <string>
#include <fstream>
#include <map>
#include <vector>

enum class SURFACE_TYPE{
	LIQUID,
	SOLID,
	BLOCKING
};

class surface{
public:
	surface(const std::string& filename);	
	SURFACE_TYPE get_type() const;	
	float get_modify() const;
	
private:
	void init_block(std::fstream &surface_file);
	
	SURFACE_TYPE	type;
	float			speed_modify;
};

class surface_area{
	using vector2D_surface_ptr = std::vector<std::vector<std::shared_ptr<surface> > >;
	using cache_surface_ptr = std::shared_ptr<file_cache<surface> >;
public:
	surface_area(const std::string& filename, cache_surface_ptr conteiner);
	
	std::shared_ptr<surface> get_surface(int x_, int y_);
	
private:
	void init_block(std::fstream 							&surface_area_file,
					cache_surface_ptr 						conteiner,
					std::map<std::string, std::string> 		&name_table);
	
	void surface_block(	std::fstream 						&surface_area_file, 
						cache_surface_ptr 					conteiner,
						std::map<std::string, std::string> 	&name_table);
			
	vector2D_surface_ptr area;
	int x_grain, y_grain;
	int x_size, y_size;
};

#endif