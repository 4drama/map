#ifndef OBSERVER__
#define OBSERVER__

#include <string>
#include <list>

#include <memory>

class observer{
public:
	void accept_event(std::string name, int value);
private:
	virtual void accept_event_core(std::string name, int value) = 0;
};

class subject{
public:
	void add_observer(std::shared_ptr<observer> obs);
	void send_event(std::string name, int value);
private:
	std::list<std::shared_ptr<observer> > observers;
};

#endif