#ifndef OBJECT__
#define OBJECT__

#include "geometry.hpp"
#include "surface.hpp"

#include <list>
#include <string>
#include <memory>

#include "interact.hpp"

class control_handler;
class interact_handler;
class graphics_handler;

class object;

class interact;
class interact_attribute;

//====================control_handler_declaration====================
class control_handler : public std::enable_shared_from_this<control_handler>{
public:
	void update(object &self, float time){
		std::shared_ptr<control_handler> control_ptr = shared_from_this();
		this->child_update(self, time);
	}
	virtual ~control_handler() = 0;
private:
	virtual void child_update(object &self, float time) = 0;
};
//-------------------------------------------------------------------


//===================interact_handler_declaration====================

/*struct interact_attribute{
	std::variant<geometry::Point, shared_ptr<object> > target;
};*/

class interact_handler{
	using Point = geometry::Point;
public:
	virtual bool walk(object &self, geometry::Point new_position) = 0;
	virtual bool check(object &self, geometry::Point target_position, double radius) = 0;
	bool check(object &self, object &other);
//	std::list<std::string> 	get_interact(object &self, interact_attribute &attr);
//	void set_interact(std::string name, std::function<void(interact_attribute&) func>);
//	void do_interact(object &self, std::string name, interact_attribute &attr);
	
	Point get_position(object &self) const;
	void set_position(object &self, geometry::Point pos);
	
	float get_speed(object &self) const;
	
	std::shared_ptr<surface> get_surface(object &self);
	void set_surface(object &self, std::shared_ptr<surface> surface_ptr);
	
	SURFACE_TYPE get_surface_type(object &self) const;
	float get_radius(object &self) const;
	
	virtual ~interact_handler() = 0;
};
//-------------------------------------------------------------------


//===================graphics_handler_declaration====================
class graphics_handler{
public:
	virtual void draw(object &self) = 0;
	
	virtual ~graphics_handler() = 0;
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
class object : public std::enable_shared_from_this<object> {
	friend class interact_handler;
public:
	
	object() = default;
	object(object_attribute &attr);
	
	void set_graphics(std::shared_ptr<graphics_handler> ptr);
	std::shared_ptr<graphics_handler> get_graphics();
	
	void set_interact(std::shared_ptr<interact_handler> ptr);	
	std::shared_ptr<interact_handler> get_interact();
	
	void set_control(std::shared_ptr<control_handler> ptr);
	std::shared_ptr<control_handler> get_control();
	
	void add_interact(std::shared_ptr<interact>);
	std::list<std::string> get_correct_interact_list(interact_attribute &attr);
	std::shared_ptr<interact> find_interact(	std::string find_command,
												interact_attribute &other_attr);
//	void interact(std::string name, interact_attribute &attr);
//	void interact
	
	void draw();
	void update(float time);
	
	virtual ~object() = default;
	
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
	
	std::list<std::shared_ptr<interact> >	interact_list;
};
//-------------------------------------------------------------------

#endif