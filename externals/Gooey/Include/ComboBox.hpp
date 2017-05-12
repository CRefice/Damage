#pragma once

#include <string>
#include <vector>

#include "Control.hpp"
#include "Event.hpp"

namespace goo
{
class ComboBox : public Control
{
public:
	ComboBox() : Control() {}
	ComboBox(const Point& pos, const Size& size) : Control(pos, size) {}

	void addItem(std::string text);
	void selectItem(int index);

	int selectedIndex() const;
	std::string selectedString() const;

	std::vector<std::string> items() const { return _items; }
	
	Event<int> onSelectionChanged;

private:
	void createControl() override;
	void doAddItem(const std::string& text);
	
	int _selectedIndex = 0;
	std::vector<std::string> _items;
};
}
