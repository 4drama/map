#ifndef OBJECT_COLLECTION__
#define OBJECT_COLLECTION__

#include "object.hpp"

#include <vector>
#include <memory>

class object;

class object_collection{
public:

	object_collection() = default;
	void add(std::shared_ptr<object> obj);
	void destroy(std::shared_ptr<object> obj);
	void update(float time);	
	void draw() const;
	
private:
	std::vector<std::shared_ptr<object> > collection;
};

#endif