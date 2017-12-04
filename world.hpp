#ifndef WORLD__
#define WORLD__

#include <SFML/Graphics.hpp>

#include "geometry.hpp"
#include "cache.hpp"
#include "tiles.hpp"
#include "keyboard.hpp"
#include "map_chank.hpp"
#include "map_collection.hpp"
//#include "character.hpp"

#include "object.hpp"
#include "graphics_handler.hpp"
#include "interact_handler.hpp"
#include "control_handler.hpp"

#include <memory>
#include <string>

#include <fstream>

#include <iostream>

class World{
public:
	World(){
		std::fstream setting("setting.txt", std::ios::in);
		std::string command;
		int res_x, res_y;
				
		while(command != "end_final"){
			setting >> command;
			if(command == "resolution"){
				setting >> res_x >> res_y;
			}			
		}
		
		this->resolution.width = res_x;
		this->resolution.height = res_y;
		this->window.create(this->resolution, "Map");
		this->view.setSize(this->resolution.width, this->resolution.height);
		this->view.setCenter(this->resolution.width/2, this->resolution.height/2);
		this->view.zoom(0.75);
		this->window.setView(view);
		
		this->window.setVerticalSyncEnabled(true);
		
		this->keyboard.add(sf::Keyboard::Left, [this](float time){
					view.move(-0.5 * time, 0);
				});			
		this->keyboard.add(sf::Keyboard::Right, [this](float time){
					view.move(0.5 * time, 0);
				});			
		this->keyboard.add(sf::Keyboard::Up, [this](float time){
					view.move(0, -0.5 * time);
				});			
		this->keyboard.add(sf::Keyboard::Down, [this](float time){
					view.move(0, 0.5 * time);
				});
		this->keyboard.add(sf::Mouse::Button::Left, [self = this](float time_arg){
					static float		time;
					static sf::Clock 	clock;
					
					time = clock.getElapsedTime().asMilliseconds();				
				
						if(time > 50 || time == 0){
							sf::Vector2i mouse_position 	= sf::Mouse::getPosition(self->window);
							sf::Vector2f centre_position	= self->view.getCenter();
							sf::Vector2f offset(	(self->resolution.width / 2) * 0.75,
													(self->resolution.height / 2) * 0.75	);
							
							sf::Vector2f world_position
									(	centre_position.x - offset.x + (mouse_position.x * 0.75),
										centre_position.y - offset.y + (mouse_position.y * 0.75));
							
							std::cerr 	<< "world_position " 
										<< world_position.x << ' ' << world_position.y << '\n';
							
						//	self->player.set_target(world_position);
						/*	self->player.get_interact()->set_target(
									self->player, 
									geometry::Point{world_position.x, world_position.y});*/
							
							std::shared_ptr<control_move> control;
							control = std::shared_ptr<control_move>(new control_move());
							control->set_target(geometry::Point{world_position.x, world_position.y});
							self->player.set_control(control);
							
							auto test = self->map.get_tile(world_position);
							
							for (auto n : test) {
							//	n->test();
							}
						}
					
					clock.restart();
				});
		
		this->cache = std::shared_ptr<Cache>(new Cache());
		this->cache_surface_ptr = std::shared_ptr<file_cache<surface> >(new file_cache<surface>());
		//this->map = Map_collection("Data/complete_map.txt", this->cache, this->cache_surface_ptr);
		
		new (&this->map) Map_collection("Data/complete_map.txt", this->cache, this->cache_surface_ptr);
		
		this->frame_texture.loadFromFile("Data/Texture/penek.png");
		this->frame_sprite.setPosition(0, 0);
		this->frame_sprite.setScale(0.75, 0.75);
		this->frame_sprite.setTexture(this->frame_texture);
		
		std::shared_ptr<window_graphics> graphics;
		std::shared_ptr<object_interact> interact;
		std::shared_ptr<control_move> control;
		graphics = std::shared_ptr<window_graphics>(new window_graphics());
		interact = std::shared_ptr<object_interact>(new object_interact());
		control = std::shared_ptr<control_move>(new control_move());
		graphics->init(&this->window);
		interact->init(&this->map);
		control->set_target(geometry::Point{200, 360});
		
		object_attribute boat_attr;
		boat_attr.position = geometry::Point{150, 150};
		boat_attr.radius = 8;
		boat_attr.speed = 0.07;
		boat_attr.surface = SURFACE_TYPE::LIQUID;
		
		boat = object(boat_attr);
		boat.set_graphics(graphics);
		boat.set_interact(interact);
		boat.set_control(control);
		
		
		object_attribute player_attr;
		player_attr.position = geometry::Point{400, 400};
		player_attr.radius = 5;
		player_attr.speed = 0.05;
		player_attr.surface = SURFACE_TYPE::SOLID;
		
		player = object(player_attr);
		player.set_graphics(graphics);
		player.set_interact(interact);
	//	player = character(400, 400);
	}
	
	void Cicle(){
		while(window.isOpen()){
			sf::Event event;
				
			while(window.pollEvent(event)){
				if (event.type == sf::Event::Closed)
					window.close();

				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
			}
			
			this->window.clear();
			this->time = this->clock.getElapsedTime().asMilliseconds();
			this->clock.restart();
			
			this->frame_sprite.setPosition(view.getCenter()
					- sf::Vector2f(	(	this->resolution.width / 2) * 0.75,
									(	this->resolution.height / 2) * 0.75));
			
			this->keyboard.update(time);
			
			this->map.draw(this->window, this->view);
			
		//	player.update(time, this->map);
		//	player.draw(this->window);
			
			boat.update(time);
			boat.draw();
			
			player.update(time);
			player.draw();
			
			this->window.draw(this->frame_sprite);
			
			this->window.setView(view);
			this->window.display();
		}
	}
	
private:
	sf::VideoMode resolution;
	sf::RenderWindow window;
	sf::View view;
	
	sf::Clock clock;
	float time;
	
	ControlFunction keyboard;
	
	std::shared_ptr<Cache> cache;
	std::shared_ptr<file_cache<surface> > cache_surface_ptr;
	Map_collection map;
	
	sf::Texture frame_texture;
	sf::Sprite frame_sprite;
	
//	character player;
	object player;
	object boat;
};

#endif