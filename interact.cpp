#include "interact.hpp"
#include "services.hpp"

#include <cassert>

#include <iostream>

std::list<std::string> get_interact_list(object &first, object &second){
	std::list<std::string> result;
	/* Need interact class which contain pair conformity.
	 * Check first and second and return all conformity.
	 */
	result.push_front("one");
	result.push_front("two");
	return result;
}

std::list<std::string> get_interact_list(object &first, std::shared_ptr<surface> second){
	
}

std::list<std::string> get_interact_list(	interact_attribute &first_attr,
											interact_attribute &second_attr){
	std::list<std::string> result;
	if(first_attr.object_ptr)
		result.merge(first_attr.object_ptr->get_correct_interact_list(second_attr));
	if(second_attr.object_ptr)
		result.merge(second_attr.object_ptr->get_correct_interact_list(first_attr));
	
	result.unique();
	
	return result;
}

std::shared_ptr<interact> find_interact(	std::string 		&command, 
											interact_attribute 	&main_attr,
											interact_attribute 	&service_attr){
	std::shared_ptr<interact> result = nullptr;
	
	if(main_attr.object_ptr){
		result = main_attr.object_ptr->find_interact(command, service_attr);
	}
	return result;
}

//==================================INTERACT===================================
interact::~interact() = default;

std::list<std::string> interact::get(interact_attribute &attr) const{
	std::list<std::string> result;
	
	for(auto &current : this->entry){
		if(this->check_core(current, attr))
			result.push_front(current);
	}
	
	return result;
}

void interact::execute(	std::string command,
						interact_attribute &active_attr,
						interact_attribute &passive_attr){
	assert(!this->entry.empty());
	
	if(this->check_core(command, active_attr))
		this->execute_core(command, active_attr, passive_attr);
}
/*
bool interact::check(std::string name, interact_attribute &attr){
	return this->check_core(name, attr);
}*/

interact::interact(std::list<std::string> entry_)
		:	entry(entry_){
}
//-----------------------------------------------------------------------------

//=============================CONTAIN_INTERACT================================
std::shared_ptr<interact> contain_interact::create(){
	std::list<std::string> entry;
	entry.push_front("in");
	entry.push_front("out");
	return std::shared_ptr<contain_interact>(new contain_interact(entry));
}

//contain_interact::contain_interact() = delete;

contain_interact::contain_interact(std::list<std::string> entry_)
		:	interact(entry_){
}

void contain_interact::execute_core(	std::string command,
										interact_attribute &active_attr,
										interact_attribute &passive_attr){
	if(command == "in"){
		if(this->check_core("in", active_attr)){
			this->object_contain = active_attr.object_ptr;
			assert(active_attr.object_ptr);
			
			std::shared_ptr<controlled_object> control_obj; 		
			control_obj = active_attr.object_ptr->get_controlled();
			passive_attr.object_ptr->set_controlled(control_obj);
			active_attr.game_services_ptr->objects().destroy(active_attr.object_ptr);
			
			std::list<std::vector<Tile>::iterator> destroy_object_tiles;
			std::shared_ptr<interact_handler> destroy_interact_handler;
			destroy_interact_handler = active_attr.object_ptr->get_interact();
			geometry::Point destroy_position =
					destroy_interact_handler->get_position(*active_attr.object_ptr);
			destroy_object_tiles =
					passive_attr.game_services_ptr->map().
					get_tile(sf::Vector2f{destroy_position.x, destroy_position.y});
			for(auto& current_tiles : destroy_object_tiles)
				current_tiles->delete_object(active_attr.object_ptr);
		}
	} else if(command == "out"){
		if(this->check_core("out", active_attr)){
			std::shared_ptr<interact_handler> object_interact;
			object_interact = this->object_contain->get_interact();
			object_interact->set_position(*this->object_contain, active_attr.position);
			passive_attr.game_services_ptr->objects().add(this->object_contain);
			
			std::shared_ptr<controlled_object> control_obj; 		
			control_obj = passive_attr.object_ptr->get_controlled();
			this->object_contain->set_controlled(control_obj);
			
			this->object_contain = nullptr;
		}
	}
}

bool contain_interact::check_core(std::string, interact_attribute &attr) const{
	return true;
}
//-----------------------------------------------------------------------------