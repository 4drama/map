#ifndef SERVICES__
#define SERVICES__

#include "keyboard.hpp"
#include "cache.hpp"
#include "file_cache.hpp"
#include "surface.hpp"
#include "map_collection.hpp"
#include "object_collection.hpp"
#include "gui.hpp"
#include "interact_gui.hpp"

#include <memory>

class services{
public:
	ControlFunction& keyboard();
	Map_collection& map();
	std::shared_ptr<Cache>& textures();
	std::shared_ptr<file_cache<surface> >& surfaces();
	object_collection& objects();
	std::shared_ptr<interact_gui>& action_dialog();
private:
	ControlFunction keyboard_;
	std::shared_ptr<Cache> texture_cache_ptr;
	std::shared_ptr<file_cache<surface> > surface_cash_ptr;
	Map_collection map_;
	object_collection objects_;
	std::shared_ptr<interact_gui> action_dialog_;
};
	 
#endif