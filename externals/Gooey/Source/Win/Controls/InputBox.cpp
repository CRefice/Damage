#include "InputBox.hpp"

namespace goo
{
void InputBox::createControl() {
	createHandle("EDIT", "", ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);
}

std::string InputBox::text() const {
	char buffer[0x100];
	::GetWindowText(handle(), buffer, 0x100);
	return std::string(buffer);
}
}
