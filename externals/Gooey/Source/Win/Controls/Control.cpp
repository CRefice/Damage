#include "Container.hpp"
#include "Control.hpp"

namespace goo
{
void Control::setVisible(bool state) {
	if(!created()) create();
	::ShowWindow(_handle, state ? SW_SHOW : SW_HIDE);
}
bool Control::visible() const {
	return ::IsWindowVisible(_handle) == TRUE;
}

void Control::setEnabled(bool state) {
	::EnableWindow(_handle, state);
}
bool Control::enabled() const {
	return ::IsWindowEnabled(_handle) == TRUE;
}

void Control::setParent(const Container& parent) {
	::SetParent(_handle, parent.handle());
	_parent = &parent;
}

void Control::setPosition(const Point& pos) {
	::MoveWindow(_handle, pos.x, pos.y, _size.x, _size.y, TRUE);
	_pos = pos;
}
void Control::setSize(const Size& size) {
	::MoveWindow(_handle, size.x, size.y, _pos.x, _pos.y, TRUE);
	_size = size;
}

void Control::setFont(Font font) {
	::SendMessage(_handle, WM_SETFONT, (WPARAM)(HFONT)(font.handle()), TRUE);
	_font = font;
}

void Control::createHandle(const char* name, const std::string& text, long style, long exStyle) {
	style |= WS_VISIBLE;
	if (_parent != nullptr) style |= WS_CHILD;

	_handle = ::CreateWindowEx(exStyle,
		name, text.c_str(), style,
		_pos.x, _pos.y, _size.x, _size.y,
		_parent ? (HWND)(_parent->handle()) : NULL,
		NULL, ::GetModuleHandle(NULL), NULL);

	if (_handle == NULL) throw std::runtime_error("Failed to create handle!");
	::SendMessage(_handle, WM_SETFONT, (WPARAM)(HFONT)(_font.handle()), TRUE);
	::SetProp(_handle, "PROP_CONTROL", this);
}
}
