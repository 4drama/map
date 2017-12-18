#ifndef INTERACT__
#define INTERACT__

#include "object.hpp"
#include "surface.hpp"
#include "object_collection.hpp"


#include <list>
#include <string>
#include <memory>

class object;
class object_collection;

std::list<std::string> get_interact_list(object &first, object &second);
std::list<std::string> get_interact_list(object &first, std::shared_ptr<surface> second);

struct interact_attribute{
	bool 								controlled_flag			= 0;
	std::shared_ptr<object> 			object_ptr				= nullptr;
	std::shared_ptr<surface> 			surface_ptr				= nullptr;
	object_collection 					*object_collection_ptr	= nullptr;
	std::shared_ptr<geometry::Point> 	position				= nullptr;
};

std::list<std::string> get_interact_list(	interact_attribute &first,
											interact_attribute &second);

class interact{
public:
	virtual ~interact() = 0;
	std::list<std::string> get(interact_attribute &attr) const;
	void execute(std::string name, interact_attribute &attr);	
//	bool check(std::string name, interact_attribute &attr);
private:
	std::list<std::string> entry;
	
	virtual void execute_core(std::string, interact_attribute &attr) = 0;
	virtual bool check_core(std::string, interact_attribute &attr) const = 0;
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
	
	void execute_core(std::string, interact_attribute &attr) override;
	bool check_core(std::string, interact_attribute &attr) const override;
};

#endif