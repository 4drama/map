#ifndef INTERACT__
#define INTERACT__

#include "object.hpp"
#include "surface.hpp"

#include <list>
#include <string>
#include <memory>

class object;
class object_collection;
class interact;
class services;

struct interact_attribute{
	std::shared_ptr<object> 			object_ptr				= nullptr;
	std::shared_ptr<surface> 			surface_ptr				= nullptr;
	services 							*game_services_ptr		= nullptr;
	geometry::Point 					position;
};

std::list<std::string> get_interact_list(	interact_attribute &first,
											interact_attribute &second);
											
std::shared_ptr<interact> find_interact(	std::string 		&command, 
											interact_attribute 	&main_attr,
											interact_attribute 	&service_attr);

class interact{
public:
	virtual ~interact() = 0;
	std::list<std::string> get(	interact_attribute &active_attr,
								interact_attribute &passive_attr) const;
	void execute(	std::string command,
					interact_attribute &active_attr,
					interact_attribute &passive_attr);	
//	bool check(std::string name, interact_attribute &attr);
private:
	std::list<std::string> entry;
	
	virtual void execute_core(	std::string command,
								interact_attribute &active_attr,
								interact_attribute &passive_attr) = 0;
	virtual bool check_core(	std::string command,
								interact_attribute &active_attr,
								interact_attribute &passive_attr) const = 0;
protected:
	interact() = delete;
	interact(std::list<std::string> entry_);
};

class contain_interact : public interact{
public:
	static std::shared_ptr<interact> create();
private:
	std::shared_ptr<object> object_contain;
	
	contain_interact() = delete;
	contain_interact(std::list<std::string> entry_);
	
	void execute_core(	std::string command,
						interact_attribute &active_attr,
						interact_attribute &passive_attr) override;
	bool check_core(	std::string command,
						interact_attribute &active_attr,
						interact_attribute &passive_attr) const override;
};

#endif