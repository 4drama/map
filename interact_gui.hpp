#ifndef	INTERACT_GUI__
#define INTERACT_GUI__

#include "gui.hpp"
#include "interact.hpp"
#include "geometry.hpp"

#include <memory>

class interact_gui{
public:
	interact_gui() = default;
	
	void execute(sf::Vector2f 		world_position);
	bool check(	sf::Vector2f		world_position) const;
	void update(sf::Vector2f		world_position);
	void init(	interact_attribute 	&&first_attr_,
				interact_attribute 	&&second_attr_,
				sf::Vector2f		world_position);
	void draw(sf::RenderWindow &window) const;
	void clean();
	
private:
	std::shared_ptr<dialog> interact_dialog;
	interact_attribute 		first_attr;
	interact_attribute 		second_attr;
};

#endif