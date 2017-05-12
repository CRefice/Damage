#pragma once

#include <vector>

#include "Control.hpp"

namespace goo
{
class Container : public Control
{
public:
	Container() : Control() {}
	Container(const Point& pos, const Size& size) : Control(pos, size) {}

	void attach(Control& control);
	//When detached, a control will be hidden,
	//but it can still be reattached later.
	void detach(Control& control);

	std::vector<Control*> children() { return _items; }

	LayoutHandle& layout() { return _layout; }

protected:
	void createChildren();

private:
	std::vector<Control*> _items;
	LayoutHandle _layout;
};
}
