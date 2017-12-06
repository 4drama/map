#ifndef OBJECT_COLLECTION__
#define OBJECT_COLLECTION__

#include "object.hpp"

#include <vector>
#include <memory>

class object_collection{
public:
	object_collection() = default;

	void add(std::shared_ptr<object> obj){
		collection.push_back(obj);
	}
	
	void update(float time){
		for (auto& current_object : collection){
			current_object->update(time);
		}
	}
	
	void draw() const{
		for (auto& current_object : collection){
			current_object->draw();
		}
	}
private:
	std::vector<std::shared_ptr<object> > collection;
};

#endif