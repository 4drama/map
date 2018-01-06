#include "control_handler.hpp"

#include <memory>

void control_move::set_target(geometry::Point position){
	this->end_position = position;		
}

void control_move::child_update(object &self, float time){
	std::shared_ptr<interact_handler> interact_ptr = self.get_interact();
	geometry::Point old_position = interact_ptr->get_position(self);
	
	if(old_position != this->end_position){
		geometry::Point new_position;
		
		interact_ptr->walk(self, old_position);
		
		float new_speed = 	interact_ptr->get_speed(self) * 
							interact_ptr->get_surface(self)->get_modify();
		
		new_position = geometry::Shift(	old_position, this->end_position, time, new_speed);
		
		if(interact_ptr->walk(self, new_position)){
			self.event("rotate", geometry::Angle(old_position, new_position));
		//	interact_ptr->set_surface(self, next_surface);
		//	interact_ptr->set_position(self, new_position);
		} else{
			std::shared_ptr<control_move> control;
			control = std::shared_ptr<control_move>(new control_move());
			control->set_target(old_position);
			self.set_control(control);
		}
	} else
		return;
}