#include "object_collection.hpp"

void object_collection::add(std::shared_ptr<object> obj){
	collection.push_back(obj);
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