#ifndef CONTROL_HANDLER__
#define CONTROL_HANDLER__

#include "object.hpp"
#include "geometry.hpp"
#include "map_collection.hpp"

class control_move : public control_handler{
public:
	control_move() = default;

	void set_target(geometry::Point position);
private:
	void child_update(object &self, float time) override;
	
	geometry::Point end_position;
};

#endif