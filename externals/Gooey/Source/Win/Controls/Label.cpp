#include "Label.hpp"

namespace goo
{
void Label::createControl() {
	createHandle("STATIC", _text, SS_LEFT);
}

void Label::setText(std::string text) {
	::SetWindowText(handle(), text.c_str());
	_text = std::move(text);
}
}
