#include "graphics.hpp"
/*
animation::animation(const animation& other)
		:	current_sprite(other.current_sprite),
			orientation_list(other.orientation_list){
	this->current_frame = this->orientation_list.begin();
}*/

void animation::init(sf::Texture *texture){
	this->current_sprite = sf::Sprite(*texture);
	this->current_sprite.setScale(1.5, 1.5);
}

void animation::add_frame(sf::IntRect rect, sf::Vector2f pivot){
	this->orientation_list.push_back(std::make_pair(rect, pivot));
	this->current_frame = this->orientation_list.begin();
}

sf::Vector2f animation::get_pivot(){
	return this->current_frame->second;
}

void animation::draw(sf::RenderWindow *window, sf::Vector2f position){
	this->current_sprite.setPosition(position);
	this->current_sprite.setTextureRect(this->current_frame->first);
	window->draw(this->current_sprite);
}

void animation::update(float time){
	static float dalay = 0;
	
	if((dalay - time) <= 0){
		this->next_frame();
		dalay = 1;
	} else 
		return;
}

void animation::next_frame(){
	if(++this->current_frame == this->orientation_list.end())
		this->current_frame = this->orientation_list.begin();
}

sf::Texture* graphics::add_texture(std::string name, std::string path){
	this->textures[name] = sf::Texture();
	this->textures[name].loadFromFile(path);
	return &this->textures[name];
}

bool graphics::check_animations(std::string name){
	auto search = this->animations.find(name);
	if(search != this->animations.end())
		return true;
    else
		return false;
}

void graphics::add_animation(std::string animation_name, animation &&new_animation){
	this->animations[animation_name] = std::move(new_animation);
}

animation& graphics::get_animation(std::string name){
	return this->animations[name];
}

void animation_manager::init(	std::shared_ptr<graphics> graphics_ptr_,
								std::string start_animation){
	this->current_animation = start_animation;
	this->graphics_ptr = graphics_ptr_;
}

void animation_manager::set_animation(std::string name){
	this->current_animation = name;
}

animation& animation_manager::get_animation(){
	return this->graphics_ptr->get_animation(this->current_animation);
}

void animation_manager::accept_event_core(std::string name, int value){
	if(name == "rotate"){
		if(	((value >= 338) && (value <= 360)) || ((value	>= 0) && (value < 23))){
			if(this->graphics_ptr->check_animations("North"))
				this->set_animation("North");
		} else if((value >= 23) && (value < 68)){
			if(this->graphics_ptr->check_animations("North_East"))
				this->set_animation("North_East");
		} else if((value >= 68) && (value < 113)){
			if(this->graphics_ptr->check_animations("East"))
				this->set_animation("East");
		} else if((value >= 113) && (value < 158)){
			if(this->graphics_ptr->check_animations("South_East"))
				this->set_animation("South_East");
		} else if((value >= 158) && (value < 203)){
			if(this->graphics_ptr->check_animations("South"))
				this->set_animation("South");
		} else if((value >= 203) && (value < 248)){
			if(this->graphics_ptr->check_animations("South_West"))
				this->set_animation("South_West");
		} else if((value >= 248) && (value < 293)){
			if(this->graphics_ptr->check_animations("West"))
				this->set_animation("West");
		} else if((value >= 293) && (value < 338)){
			if(this->graphics_ptr->check_animations("North_West"))
				this->set_animation("North_West");
		}
	}
}