#include "services.hpp"

ControlFunction& services::keyboard(){
	return keyboard_;
}

Map_collection& services::map(){
	return map_;
}

std::shared_ptr<Cache>& services::textures(){
	return texture_cache_ptr;
}

std::shared_ptr<file_cache<surface> >& services::surfaces(){
	return surface_cash_ptr;
}

object_collection& services::objects(){
	return objects_;
}

std::shared_ptr<interact_gui>& services::action_dialog(){
	return action_dialog_;
}