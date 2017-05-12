#include "ComboBox.hpp"

namespace goo
{
void ComboBox::createControl() {
	createHandle(WC_COMBOBOX, "", CBS_DROPDOWNLIST | CBS_HASSTRINGS);
	for (auto& str : _items) {
		doAddItem(str);
	}
	selectItem(_selectedIndex);
}

void ComboBox::addItem(std::string text) {
	doAddItem(text);
	_items.push_back(std::move(text));
}

void ComboBox::doAddItem(const std::string& text) {
	::SendMessage(handle(), CB_ADDSTRING, 0, (LPARAM)(text.c_str()));
}

int ComboBox::selectedIndex() const {
	return (int)(::SendMessage(handle(), CB_GETCURSEL, 0, 0));
}

void ComboBox::selectItem(int index) {
	::SendMessage(handle(), CB_SETCURSEL, index, 0);
	_selectedIndex = index;
}

std::string ComboBox::selectedString() const {
	char buffer[0x100];
	::SendMessage(handle(), CB_GETLBTEXT, selectedIndex(), (LPARAM)buffer);
	return std::string(buffer);
}
}
