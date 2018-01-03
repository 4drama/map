#include "world.hpp"

#include "geometry.hpp"
#include "tiles.hpp"
#include "map_chank.hpp"
#include "object.hpp"
#include "graphics_handler.hpp"
#include "interact_handler.hpp"
#include "control_handler.hpp"
#include "graphics.hpp"

#include "interact.hpp"


#include <fstream>
#include <string>

#include "mouse.hpp"

#include <iostream>

World::World(){
	using interact_type = interact;
	using graphics_type = graphics;
	
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
	
	this->game.keyboard().add(sf::Keyboard::Left, [this](float time){
				view.move(-0.5 * time, 0);
			});			
	this->game.keyboard().add(sf::Keyboard::Right, [this](float time){
				view.move(0.5 * time, 0);
			});			
	this->game.keyboard().add(sf::Keyboard::Up, [this](float time){
				view.move(0, -0.5 * time);
			});			
	this->game.keyboard().add(sf::Keyboard::Down, [this](float time){
				view.move(0, 0.5 * time);
			});
	this->game.keyboard().add(sf::Mouse::Button::Left, [self = this](float time_arg){
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
						
						if(self->game.action_dialog()->check(world_position)){
							self->game.action_dialog()->execute(world_position);
							std::cerr << "execute" << std::endl;
						} else {
							std::shared_ptr<control_move> control;
							control = std::shared_ptr<control_move>(new control_move());
							control->set_target(geometry::Point{world_position.x, world_position.y});
							
							std::shared_ptr<object> current_object;
							if(current_object = self->player->get_object()){
								current_object->set_control(control);
							}
						}
						self->game.action_dialog()->clean();
					}
				
				clock.restart();
			});
	this->game.keyboard().add(sf::Mouse::Button::Right, [self = this](float time_arg){
				static float		time;
				static sf::Clock 	clock;
				
				time = clock.getElapsedTime().asMilliseconds();				
			
					if(time > 50 || time == 0){
						self->game.action_dialog()->clean();
						
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
						

						objects = self->game.map().get_objects(world_position);
						
						std::shared_ptr<object> player;
						std::shared_ptr<interact_handler> player_interact;
						if(player = self->player->get_object())
							player_interact = player->get_interact();
						else
							return;
						
						interact_attribute player_attr;
						player_attr.object_ptr = player;
						player_attr.game_services_ptr = &self->game;
						
						geometry::Point world_position_point;
						world_position_point = geometry::Point{	world_position.x,
								world_position.y};
						
						if(objects.empty()){
							if(player_interact->check(*player, world_position_point, 10)){
								std::list<std::vector<Tile>::iterator> current_tiles;
								current_tiles = self->game.map().get_tile(world_position);
								
								interact_attribute surface_attr;
								surface_attr.surface_ptr =
										current_tiles.front()->get_surface(world_position);
								
								surface_attr.position = world_position_point;
								
								self->game.action_dialog()->init(	std::move(player_attr),
										std::move(surface_attr),
										world_position);							
							}
						} else {
							for (auto& obj : objects){
								if(player_interact->check(*player, *obj)){
									
									interact_attribute other_attr;
									other_attr.object_ptr = obj;
									other_attr.game_services_ptr = &self->game;
									
									std::cerr << "interact" << std::endl;
									self->game.action_dialog()->init(	std::move(player_attr),
														std::move(other_attr),
														world_position);
								}
							}
						}
					}
				
				clock.restart();
			});
	
	
	this->game.textures() = std::shared_ptr<Cache>(new Cache());
	this->game.surfaces() = std::shared_ptr<file_cache<surface> >(new file_cache<surface>());
	
	new (&this->game.map()) Map_collection("Data/complete_map.txt", this->game.textures(), this->game.surfaces());
	
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
	interact->init(&this->game.map());
	control->set_target(geometry::Point{205, 365});
	
	object_attribute boat_attr;
	boat_attr.position = geometry::Point{150, 150};
	boat_attr.radius = 8;
	boat_attr.speed = 0.07;
	boat_attr.surface = SURFACE_TYPE::LIQUID;
			
	std::shared_ptr<object> boat = std::shared_ptr<object>(new object(boat_attr));
//	boat->set_graphics(graphics);
	auto anim_graphics = std::shared_ptr<animation_graphics>(new animation_graphics());
	anim_graphics->init(&this->window);
	boat->set_graphics(anim_graphics);
	
	auto boat_anim_manager = std::shared_ptr<animation_manager>(new animation_manager());
	auto boat_graphics = std::shared_ptr<graphics_type>(new graphics_type());
	sf::Texture* main_boat_texture;
	main_boat_texture = boat_graphics->add_texture("main", "Data/Texture/boat.png");
	sf::IntRect rect(0, 0, 40, 40);
	animation left_animation{};
	left_animation.init(main_boat_texture);
	left_animation.add_frame(rect, sf::Vector2f{28, 42});
	boat_graphics->add_animation("Left", std::move(left_animation));
	
	boat_anim_manager->init(boat_graphics, "Left");
	boat->set_animation_manager(boat_anim_manager);
	
	boat->set_interact(interact);
	boat->set_control(control);
	std::shared_ptr<interact_type> boat_interact;
	boat_interact = contain_interact::create();
	boat->add_interact(boat_interact);
	
	
	object_attribute player_attr;
	player_attr.position = geometry::Point{250, 350};
	player_attr.radius = 5;
	player_attr.speed = 0.05;
	player_attr.surface = SURFACE_TYPE::SOLID;
	
	std::shared_ptr<object> player = std::shared_ptr<object>(new object(player_attr));
	player->set_graphics(graphics);
	player->set_interact(interact);
	this->player = std::shared_ptr<controlled_object>(new controlled_object());
	player->set_controlled(this->player);
	
	player_attr.position = geometry::Point{420, 450};
	
	std::shared_ptr<object> npc = std::shared_ptr<object>(new object(player_attr));
	npc->set_graphics(graphics);
	npc->set_interact(interact);
	
	control->update(*player , 0);
	control->update(*npc , 0);

	this->game.objects().add(boat->shared_from_this());
	this->game.objects().add(player->shared_from_this());				
	this->game.objects().add(npc->shared_from_this());
//	this->player = player;
	
	this->game.action_dialog() = std::shared_ptr<interact_gui>(new interact_gui());
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
		
		this->game.keyboard().update(time);
		
		this->game.map().draw(this->window, this->view);
					
		this->game.objects().update(time);
		this->game.objects().draw();
		
		this->game.action_dialog()->draw(this->window);
		this->game.action_dialog()->update(mouse_world_position(this->window, this->view));
		
		this->window.draw(this->frame_sprite);
		
		this->window.setView(view);
		this->window.display();
	}
}