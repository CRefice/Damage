#include "GroupBox.hpp"

namespace goo
{
void GroupBox::createControl() {
	createHandle("BUTTON", _text, BS_GROUPBOX);
}

void GroupBox::setText(std::string text)
{
	::SetWindowText(handle(), text.c_str());
	_text = std::move(text);
}
}
