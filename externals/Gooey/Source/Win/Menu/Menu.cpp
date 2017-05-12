#include <algorithm>

#include "Menu.hpp"

namespace goo
{
static int totalId = 0;

//Classic C++ trick: wrapping a static in a function
static std::vector<MenuItem*>& staticVec() {
  static std::vector<MenuItem*> vec;
  return vec;
}

MenuItem::MenuItem(std::string text, long style) : _text(std::move(text)), _style(style), _id(totalId++) {}
MenuItem::MenuItem(std::string text, long style, uintptr_t id) : _text(text), _style(style), _id(id) {}

TextItem::TextItem(std::string text) : MenuItem(std::move(text), MF_STRING) {}

void TextItem::setChecked(bool state) {
	::CheckMenuItem(_container, (UINT)_id, state ? MF_CHECKED : MF_UNCHECKED);
	_checked = state;
}

SeparatorItem::SeparatorItem() : MenuItem(nullptr, MF_SEPARATOR, 0) {}


Menu::Menu() : _handle(::CreateMenu()) {}

void Menu::appendItem(MenuItem& item) {
	::AppendMenu(_handle, item._style, item._id, item._text.c_str());
	item._container = _handle;
	staticVec().emplace_back(&item);
}

MenuItem* Menu::itemFromId(int id) {
	auto item = std::find_if(staticVec().cbegin(), staticVec().cend(),
		[this, id](MenuItem* item) {
			return item->_id == id;
		});

	return item != staticVec().end() ? *item : nullptr;
}

PopupMenuItem::PopupMenuItem(std::string text) : Menu(), MenuItem(std::move(text), MF_POPUP, (UINT_PTR)(HMENU)handle()) {}
}
