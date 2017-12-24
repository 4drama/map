#include "tiles.hpp"


//================================Tile=========================================
Tile::Tile(int x_pos_, int y_pos_, std::shared_ptr<Tile_base> base_)
		:	state(State::outView), base(base_){
	sprite.setPosition(x_pos_, y_pos_);
}
	
void Tile::draw(sf::RenderWindow &window, sf::View &view){
	if(state == State::outView){
		if(view_check(view)){
			state = State::inView;
			texture = base->conteiner->load(base->filename);
			sprite.setTexture(*texture);
			this->draw(window, view);
			return;
		} else {
			return;
		}			
	} else if(state == State::inView){
		if(view_check(view)){
			window.draw(sprite);
			return;
		} else {
			state = State::outView;
			texture = nullptr;
			return;
		}
	}
}

std::shared_ptr<surface> Tile::get_surface(sf::Vector2f point){
	return base->area.get_surface(point.x, point.y);
}

bool Tile::view_check(sf::View &view){
	const sf::Vector2f pos = view.getCenter();
	const sf::Vector2f sz = view.getSize();
	const sf::Vector2f sprite_position = sprite.getPosition();
	
	
	sf::FloatRect camera(pos.x-(sz.x/2)*-1 , pos.y-(sz.y/2) , sz.x, sz.y);

	if(	(camera.left+base->x_size < sprite_position.x) ||
		(camera.left-camera.width-(base->x_size*2) > sprite_position.x) ||
		(camera.top+base->y_size+camera.height < sprite_position.y) ||
		(camera.top-(base->y_size*2) > sprite_position.y)){
		
		return false;
	} else {
		return true;
	} 
}

bool Tile::check_position(const sf::Vector2f point){
/*	if(state == State::outView){
		this->texture = base->conteiner->load(base->filename);
		this->sprite.setTexture(*texture);
	}*/
	
	sf::Vector2f position = this->sprite.getPosition();
	sf::Vector2u size{(unsigned)base->x_size, (unsigned)base->y_size};
/*	if(this->texture != nullptr)
		size = this->texture->getSize();
	else
		return false;*/
	
	if(	((point.x >= position.x) && (point.x <= (position.x + size.x)))	&&
		((point.y >= position.y) && (point.y <= (position.y + size.y)))	){
		
		return true;
	}
	else {
		return false;
	}
}

void Tile::add_object(object &obj){
	objects.push_front(obj.shared_from_this());
}

void Tile::remove_object(object &obj){
	objects.remove_if([need = obj.shared_from_this()](std::weak_ptr<object> current){
			return (current.lock() == need) ? true : false;
		});
}

std::list<std::shared_ptr<object> > Tile::get_objects() const {
	std::list<std::shared_ptr<object> > result;
	for (auto& current : objects){
		std::shared_ptr<object> obj;
		if(obj = current.lock())
			result.push_front(obj);
	}
	return result;
}

void Tile::delete_object(std::shared_ptr<object> object_){
	for (auto it = this->objects.begin(); it != this->objects.end(); ++it){
		if(object_ == it->lock()){
			this->objects.erase(it);
		}
	}
}
//-----------------------------------------------------------------------------

//=============================Tile_base=======================================
Tile_base::Tile_base(	const std::string &filename_, const std::string &surface_path,
						int x_size_, int y_size_,
						std::shared_ptr<Cache> &conteiner_, 
						std::shared_ptr<file_cache<surface> > cache_surface_ptr)
		:	conteiner(conteiner_), area(surface_path, cache_surface_ptr),
			filename(filename_), x_size(x_size_), y_size(y_size_){	
}

Tile Tile_base::create_tile(int x_position, int y_position){
	return Tile(x_position, y_position, shared_from_this());
}
//-----------------------------------------------------------------------------