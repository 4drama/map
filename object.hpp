#ifndef OBJECT__
#define OBJECT__

#include "geometry.hpp"
#include "surface.hpp"

#include <memory>

class control_handler;
class interact_handler;
class graphics_handler;

class object;

//====================control_handler_declaration====================
class control_handler{
public:
	
};
//-------------------------------------------------------------------


//===================interact_handler_declaration====================
class interact_handler{
	using Point = geometry::Point;
public:
	virtual void update(object &self, float time) = 0;
	
	Point get_position(object &self) const;
	void set_position(object &self, geometry::Point pos);

	Point get_target(object &self) const;
	void set_target(object &self, geometry::Point target);
	
	float get_speed(object &self) const;
	
	std::shared_ptr<surface> get_surface(object &self);
	void set_surface(object &self, std::shared_ptr<surface> surface_ptr);
	
	SURFACE_TYPE get_surface_type(object &self) const;
	float get_radius(object &self) const;
};
//-------------------------------------------------------------------


//===================graphics_handler_declaration====================
class graphics_handler{
public:
	virtual void draw(object &self) = 0;
};
//-------------------------------------------------------------------


//==================object_attribute_declaration=====================
struct object_attribute{
	using Point = geometry::Point;
	
	Point 			position;
	float			radius;
	float			speed;
	SURFACE_TYPE	surface;
};
//-------------------------------------------------------------------


//=======================object_declaration==========================
class object{
	friend class interact_handler;
public:
	object() = default;
	object(object_attribute &attr);
	
	void set_graphics(std::shared_ptr<graphics_handler> ptr);
	std::shared_ptr<graphics_handler> get_graphics();
	
	void set_interact(std::shared_ptr<interact_handler> ptr);	
	std::shared_ptr<interact_handler> get_interact();
	
	void draw();
	void update(float time);
	
private:
	virtual void child_draw();	
	virtual void child_update(float time);

	geometry::Point 						position;
	float									collision_radius;
	
	geometry::Point 						target_position;
	float 									speed;
	
	std::shared_ptr<surface> 				current_surface;
	SURFACE_TYPE							surface_type;
	
	std::shared_ptr<control_handler>		control_ptr;
	std::shared_ptr<interact_handler>		interact_ptr;
	std::shared_ptr<graphics_handler>		graphics_ptr;
	
};
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

void object::draw(){
	if(graphics_ptr)
		graphics_ptr->draw(*this);
	this->child_draw();
}

void object::update(float time){
	if(interact_ptr)	
		interact_ptr->update(*this, time);
	
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
geometry::Point interact_handler::get_position(object &self) const{
	return self.position;
}

void interact_handler::set_position(object &self, geometry::Point pos){
	self.position = pos;
}

geometry::Point interact_handler::get_target(object &self) const{
	return self.target_position;
}

void interact_handler::set_target(object &self, geometry::Point target){
	self.target_position = target;
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

#endif