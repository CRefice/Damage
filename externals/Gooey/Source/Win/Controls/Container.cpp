#include <algorithm>

#include "Container.hpp"

namespace goo
{
void Container::attach(Control& control) {
	control.setParent(*this);
	if (created() && !control.created()) {
		control.create();
	}
	_items.emplace_back(&control);
}
void Container::detach(Control& control) {
	control.hide();
	_items.erase(std::remove_if(_items.begin(), _items.end(), [&control](Control* ctrl) {
		return ctrl == (&control); 
	}), _items.end());
}
void Container::createChildren() {
	for(auto& control : _items) {
		 if(!control->created()) {
			 control->create();
		 }
	}
}
}
