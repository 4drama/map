#include "interact_gui.hpp"

#include <SFML/Graphics.hpp>

#include <cassert>

void interact_gui::execute(sf::Vector2f 		world_position){	
	std::string entry = this->interact_dialog->get(world_position);
	std::shared_ptr<interact> current_interact;
	current_interact = find_interact(entry, this->first_attr, this->second_attr);	
	if(!current_interact){
		current_interact = find_interact(entry, this->second_attr, this->first_attr);
		assert(current_interact);
		current_interact->execute(entry, this->first_attr, this->second_attr);
	} else{
		current_interact->execute(entry, this->second_attr, this->first_attr);
	}	
}

bool interact_gui::check(sf::Vector2f	world_position) const{
	if(	(this->interact_dialog != nullptr) && 
		(this->interact_dialog->check(world_position)))
		return true;
	else
		return false;
}

void interact_gui::draw(sf::RenderWindow &window) const{
	if(this->interact_dialog)
		this->interact_dialog->draw(window);
}

void interact_gui::clean(){
	this->interact_dialog = nullptr;
}

void interact_gui::update(sf::Vector2f world_position){
	if(this->interact_dialog)
		this->interact_dialog->update(world_position);
}

void interact_gui::init(	interact_attribute	&&first_attr_,
							interact_attribute	&&second_attr_,
							sf::Vector2f world_position){

	this->first_attr = std::move(first_attr_);
	this->second_attr = std::move(second_attr_);
	
	std::list<std::string> interact_dialog_list;
	interact_dialog_list = get_interact_list(this->first_attr, this->second_attr);

	this->interact_dialog = std::shared_ptr<dialog>(new dialog
			(world_position, interact_dialog_list));
}