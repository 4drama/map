#include "interact.hpp"

#include <cassert>

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

void interact::execute(std::string name, interact_attribute &attr){
	assert(!this->entry.empty());
	
	if(this->check_core(name, attr))
		this->execute_core(name, attr);
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

void contain_interact::execute_core(std::string command, interact_attribute &attr){
	if(command == "in"){
		if(this->check_core("in", attr)){
			this->object_contain = attr.object_ptr;
			assert(attr.object_ptr);
			attr.object_collection_ptr->destroy(attr.object_ptr);
		}
	} else if(command == "out"){
		if(this->check_core("out", attr)){
			std::shared_ptr<interact_handler> object_interact;
			object_interact = this->object_contain->get_interact();
			object_interact->set_position(*this->object_contain, *attr.position);
			
			attr.object_collection_ptr->add(this->object_contain);
			this->object_contain = nullptr;
		}
	}
}

bool contain_interact::check_core(std::string, interact_attribute &attr) const{
	return true;
}
//-----------------------------------------------------------------------------