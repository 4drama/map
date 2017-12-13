#include "surface.hpp"

#include <iostream>

surface::surface(const std::string& filename){
//		std::cerr << filename << std::endl;
	std::fstream surface_file(filename, std::ios::in);
	std::string command;
	while(command != "end_final"){
		surface_file >> command;
		if(command == "init_block"){
			this->init_block(surface_file);
		}
	}
}

SURFACE_TYPE surface::get_type() const {
	return type;
}

float surface::get_modify() const{
	return speed_modify;
}

void surface::init_block(std::fstream &surface_file){
	std::string command;
	while(command != "end_block"){
		surface_file >> command;
		if(command == "type"){
			std::string type_name;
			surface_file >> type_name;
			if(type_name == "liquid"){
				this->type = SURFACE_TYPE::LIQUID;
			} else if(type_name == "solid"){
				this->type = SURFACE_TYPE::SOLID;
			} else if(type_name == "blocking"){
				this->type = SURFACE_TYPE::BLOCKING;
			}
		} else if(command == "speed_modify"){
			surface_file >> this->speed_modify;
//				std::cerr << this->speed_modify << std::endl;
		}
	}
}

surface_area::surface_area(const std::string& filename, cache_surface_ptr conteiner){
	std::fstream surface_area_file(filename, std::ios::in);
	std::string command;
	std::map<std::string, std::string> name_table;
	
	while(command != "end_final"){
		surface_area_file >> command;
		if(command == "init_block"){
			this->init_block(surface_area_file, conteiner, name_table);
		} else if(command == "surface_block"){
			this->surface_block(surface_area_file, conteiner, name_table);
		}
	}
}


std::shared_ptr<surface> surface_area::get_surface(int x_, int y_){
	int x_index = (x_ % this->x_size) / (this->x_size / x_grain); //- (x_ % this->x);
	int y_index = (y_ % this->y_size) / (this->y_size / y_grain); // - (y_ % this->y);
	
//	std::cerr << x_index << ' ' << y_index << std::endl;
	return area[y_index][x_index];
}

void surface_area::init_block(std::fstream &surface_area_file, cache_surface_ptr conteiner,
		std::map<std::string, std::string> &name_table){
			
	std::string command;
	
	while(command != "end_block"){
		surface_area_file >> command;
		if(command == "grain"){
			surface_area_file >> this->x_grain >> this->y_grain;
			area.resize(this->x_grain);
			for(auto i : area){
			//	i = std::vector<std::shared_ptr<surface> >();
				i.reserve(this->y_grain);
			}
			//	i.resize(this->y);
			//	i.reserve(this->y);
		} else if(command == "add"){
			std::string name;
			std::string path;
			surface_area_file >> name >> path ;
			name_table[name] = path;
		} else if(command == "pixels"){
			surface_area_file >> x_size >> y_size ;
		}
	}
}

void surface_area::surface_block(std::fstream &surface_area_file, cache_surface_ptr conteiner,
		std::map<std::string, std::string> &name_table){
			
	std::string command;
	surface_area_file >> command;
	
	int 	x_index = 0,
			y_index = 0;
	
//	std::cerr << "size: " << area.size() << std::endl;
	
	do{	
		if(command == "end_lines"){
			y_index ++;
			x_index = 0;
		//	std::cerr << std::endl;
		} else {
			auto search = name_table.find(command);
			if(search != name_table.end())
			//	area[y_index][x_index] = conteiner->load(name_table[command]);
				area.at(y_index).push_back(conteiner->load(name_table[command]));
		//	std::cerr << name_table[command] << std::endl;
			
		//	auto ttd = conteiner->load(name_table[command]);
		//	std::cerr << '[' << y_index << ':' << x_index << ']' << name_table[command];
			x_index ++;
		}
		
		surface_area_file >> command;
	} while (command != "end_block");
//	std::cerr << std::endl;
}