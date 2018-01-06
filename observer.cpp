#include "observer.hpp"

//#include <iostream>

void observer::accept_event(std::string name, int value){
//	std::cerr << "debug: msg " <<  name << ". val " << value << '.' << std::endl;
	this->accept_event_core(name, value);
}

void subject::add_observer(std::shared_ptr<observer> obs){
	this->observers.push_front(obs);
}

void subject::send_event(std::string name, int value){
	for(auto& current_observer : this->observers){
		current_observer->accept_event(name, value);
	}
}