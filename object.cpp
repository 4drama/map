#include "object.hpp"

//#include <cassert>

//===================controlled_object_implementation================
std::shared_ptr<object> controlled_object::get_object(){	
	return this->obj.lock();
}

void controlled_object::set_object(std::shared_ptr<object> obj_){
	this->obj = obj_;
}
//-------------------------------------------------------------------


//=======================object_implementation=======================
object::object(object_attribute &attr)
		:	position{attr.position},
			collision_radius(attr.radius),
			target_position(position),
			speed(attr.speed),
			surface_type(attr.surface){
}

void object::set_graphics(std::shared_ptr<graphics_handler> ptr){
	h_graphics_ptr = ptr;
}

std::shared_ptr<graphics_handler> object::get_graphics(){
	return this->h_graphics_ptr;
}

void object::set_interact(std::shared_ptr<interact_handler> ptr){
	h_interact_ptr = ptr;
}

std::shared_ptr<interact_handler> object::get_interact(){
	return h_interact_ptr;
}

void object::set_control(std::shared_ptr<control_handler> ptr){
	h_control_ptr = ptr;
}

std::shared_ptr<control_handler> object::get_control(){
	return h_control_ptr;
}

void object::add_interact(std::shared_ptr<interact> new_interact){
	this->interact_list.push_front(new_interact);
}

std::list<std::string> object::get_correct_interact_list(interact_attribute &other_attr){
	std::list<std::string> result;
	
	interact_attribute self_attr;
	self_attr.object_ptr = this->shared_from_this();
	self_attr.game_services_ptr = other_attr.game_services_ptr;
	
	for(auto& current : this->interact_list){
		result.merge(current->get(self_attr, other_attr));
	}
	
	return result;
}
/*
void object::interact(std::string name, interact_attribute &attr){
	interact_attribute null_atr;
//	std::shared_ptr<interact> current =  find_interact(name, null_atr, );
}*/

std::shared_ptr<interact> object::find_interact(	std::string find_command, 
													interact_attribute &other_attr){
	for(auto& current_interact : this->interact_list){
		for(std::string current_command : this->get_correct_interact_list(other_attr)){
			if(current_command == find_command)
				return current_interact;
		}
	}
	return nullptr;
}

std::shared_ptr<controlled_object> object::get_controlled(){
	return this->owner_ptr;
}

void object::set_controlled(std::shared_ptr<controlled_object> owner){
	if(owner != nullptr){
		if(auto old_obj = owner->get_object())
			old_obj->set_controlled(nullptr);
		this->owner_ptr = owner;
		this->owner_ptr->set_object(this->shared_from_this());
	} else 
		this->owner_ptr = nullptr;
}

std::shared_ptr<animation_manager> object::get_animation_manager(){
	return this->animation_ptr;
}

void object::set_animation_manager(std::shared_ptr<animation_manager> animation_manager_ptr){
	this->animation_ptr = animation_manager_ptr;
	this->observers.add_observer(this->animation_ptr);
}

void object::draw(){
	if(h_graphics_ptr)
		h_graphics_ptr->draw(*this);
	this->child_draw();
}

void object::update(float time){
	if(h_control_ptr)
		h_control_ptr->update(*this, time);
	
	if(animation_ptr)
		animation_ptr->get_animation().update(time);
	
	this->child_update(time);
}

void object::event(std::string msg, int value){
	this->observers.send_event(msg, value);
}

void object::child_draw(){
	return ;
}

void object::child_update(float time){
	return ;
}

//-------------------------------------------------------------------


//=================interact_handler_implementation===================
bool interact_handler::check(object &self, object &other){
	std::shared_ptr<interact_handler> other_handler = other.get_interact();
	geometry::Point target_position = other_handler->get_position(other);
	double radius = other_handler->get_radius(other);
	return this->check(self, target_position, radius);
}

geometry::Point interact_handler::get_position(object &self) const{
	return self.position;
}

void interact_handler::set_position(object &self, geometry::Point pos){
	self.position = pos;
}

float interact_handler::get_speed(object &self) const{
	return self.speed;
}

std::shared_ptr<surface> interact_handler::get_surface(object &self){
	return self.current_surface;
}

void interact_handler::set_surface(object &self, std::shared_ptr<surface> surface_ptr){
	self.current_surface = surface_ptr;
}

SURFACE_TYPE interact_handler::get_surface_type(object &self) const{
	return self.surface_type;
}

float interact_handler::get_radius(object &self) const{
	return self.collision_radius;
}
//-------------------------------------------------------------------


//===================================================================
control_handler::~control_handler() = default;
interact_handler::~interact_handler() = default;
graphics_handler::~graphics_handler() = default;
//-------------------------------------------------------------------
