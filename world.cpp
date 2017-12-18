#include "world.hpp"

#include "geometry.hpp"
#include "tiles.hpp"
#include "map_chank.hpp"
#include "object.hpp"
#include "graphics_handler.hpp"
#include "interact_handler.hpp"
#include "control_handler.hpp"

#include "interact.hpp"

#include <fstream>
#include <string>

#include "mouse.hpp"

#include <iostream>

World::World(){
	using interact_type = interact;
	
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
			//			self->one.reset();
					//	self->two = nullptr;
						
						sf::Vector2i mouse_position 	= sf::Mouse::getPosition(self->window);
						sf::Vector2f centre_position	= self->view.getCenter();
						sf::Vector2f offset(	(self->resolution.width / 2) * 0.75,
												(self->resolution.height / 2) * 0.75	);
						
						sf::Vector2f world_position
								(	centre_position.x - offset.x + (mouse_position.x * 0.75),
									centre_position.y - offset.y + (mouse_position.y * 0.75));
						
						std::cerr 	<< "world_position " 
									<< world_position.x << ' ' << world_position.y << '\n';
						
						if((self->two != nullptr) && self->two->check(world_position)){
							std::cerr << self->two->get(world_position) << std::endl;
							
						} else {
							std::shared_ptr<control_move> control;
							control = std::shared_ptr<control_move>(new control_move());
							control->set_target(geometry::Point{world_position.x, world_position.y});
							
							std::shared_ptr<object> current_object;
							if(current_object = self->player.lock())
								current_object->set_control(control);
						}
						self->two = nullptr;
					}
				
				clock.restart();
			});
	this->keyboard.add(sf::Mouse::Button::Right, [self = this](float time_arg){
				static float		time;
				static sf::Clock 	clock;
				
				time = clock.getElapsedTime().asMilliseconds();				
			
					if(time > 50 || time == 0){
			//			self->one.reset();
						self->two = nullptr;
						
						sf::Vector2i mouse_position 	= sf::Mouse::getPosition(self->window);
						sf::Vector2f centre_position	= self->view.getCenter();
						sf::Vector2f offset(	(self->resolution.width / 2) * 0.75,
												(self->resolution.height / 2) * 0.75	);
						
						sf::Vector2f world_position
								(	centre_position.x - offset.x + (mouse_position.x * 0.75),
									centre_position.y - offset.y + (mouse_position.y * 0.75));
						
						std::cerr 	<< "world_position " 
									<< world_position.x << ' ' << world_position.y << '\n';
						
						std::list<std::shared_ptr<object> > objects;
						

						objects = self->map.get_objects(world_position);
						
						if(objects.empty()){
							/* surface interact */
						} else {
						
							std::shared_ptr<object> player;
							std::shared_ptr<interact_handler> player_interact;
							if(player = self->player.lock())
								player_interact = player->get_interact();
							else
								return;
							
							for (auto& obj : objects){
								if(player_interact->check(*player, *obj)){
									interact_attribute player_attr;
									player_attr.controlled_flag = 1;
									player_attr.object_ptr = player;
									player_attr.object_collection_ptr = &self->objects;
									
									interact_attribute other_attr;
									player_attr.object_ptr = obj;
									player_attr.object_collection_ptr = &self->objects;
									
									std::cerr << "interact" << std::endl;
									std::list<std::string> interact_dialog;
								//	interact_dialog = get_interact_list(*player, *obj);
									interact_dialog = get_interact_list(player_attr, other_attr);

									self->two = std::shared_ptr<dialog>(
											new dialog(world_position, interact_dialog));
				//					self->one = dialog(mouse_position.x * 0.75, mouse_position.y * 0.75);
								}
							}
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
	control->set_target(geometry::Point{205, 365});
	
	object_attribute boat_attr;
	boat_attr.position = geometry::Point{150, 150};
	boat_attr.radius = 8;
	boat_attr.speed = 0.07;
	boat_attr.surface = SURFACE_TYPE::LIQUID;
			
	std::shared_ptr<object> boat = std::shared_ptr<object>(new object(boat_attr));
	boat->set_graphics(graphics);
	boat->set_interact(interact);
	boat->set_control(control);
	std::shared_ptr<interact_type> boat_interact;
	boat_interact = contain_interact::create();
	boat->add_interact(boat_interact);
	
	
	object_attribute player_attr;
	player_attr.position = geometry::Point{400, 400};
	player_attr.radius = 5;
	player_attr.speed = 0.05;
	player_attr.surface = SURFACE_TYPE::SOLID;
	
	std::shared_ptr<object> player = std::shared_ptr<object>(new object(player_attr));
	player->set_graphics(graphics);
	player->set_interact(interact);
	
	player_attr.position = geometry::Point{420, 450};
	
	std::shared_ptr<object> npc = std::shared_ptr<object>(new object(player_attr));
	npc->set_graphics(graphics);
	npc->set_interact(interact);
	
//		control->update(*boat , 0);
	control->update(*player , 0);
	control->update(*npc , 0);

	objects.add(boat->shared_from_this());
	objects.add(player->shared_from_this());				
	objects.add(npc->shared_from_this());
	this->player = player;
	
}

void World::Cicle(){
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
					
		objects.update(time);
		objects.draw();
		
		
//		one.draw(this->window, this->view);
//		one.update(time, this->window, this->view);
		
		if(two != nullptr){
			two->draw(this->window);
			two->update(mouse_world_position(this->window, this->view));
		}
		
		this->window.draw(this->frame_sprite);
		
		this->window.setView(view);
		this->window.display();
	}
}