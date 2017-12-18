#include "object_collection.hpp"

void object_collection::add(std::shared_ptr<object> obj){
	for(auto& curr_obj : collection){
		if(curr_obj == nullptr){
			curr_obj = obj;
			return;
		}
	}
	collection.push_back(obj);
}

void object_collection::destroy(std::shared_ptr<object> obj_to_del){
	for(auto& curr_obj : collection){
		if(curr_obj == obj_to_del)
			curr_obj = nullptr;
	}
}

void object_collection::update(float time){
	for (auto& current_object : collection){
		current_object->update(time);
	}
}

void object_collection::draw() const{
	for (auto& current_object : collection){
		current_object->draw();
	}
}