#include "object.hpp"

//=======================object_implementation=======================
object::object(object_attribute &attr)
		:	position{attr.position},
			collision_radius(attr.radius),
			target_position(position),
			speed(attr.speed),
			surface_type(attr.surface){
}

void object::set_graphics(std::shared_ptr<graphics_handler> ptr){
	graphics_ptr = ptr;
}

std::shared_ptr<graphics_handler> object::get_graphics(){
	return this->graphics_ptr;
}

void object::set_interact(std::shared_ptr<interact_handler> ptr){
	interact_ptr = ptr;
}

std::shared_ptr<interact_handler> object::get_interact(){
	return interact_ptr;
}

void object::set_control(std::shared_ptr<control_handler> ptr){
	control_ptr = ptr;
}

std::shared_ptr<control_handler> object::get_control(){
	return control_ptr;
}

void object::draw(){
	if(graphics_ptr)
		graphics_ptr->draw(*this);
	this->child_draw();
}

void object::update(float time){
	if(control_ptr)
		control_ptr->update(*this, time);
	
	this->child_update(time);
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
