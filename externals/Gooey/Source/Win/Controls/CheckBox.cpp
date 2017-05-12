#include "CheckBox.hpp"

namespace goo
{
void CheckBox::createControl() {
	createHandle("BUTTON", _text, BS_AUTOCHECKBOX);
}

void CheckBox::setChecked(bool state) {
	::SendMessage(handle(), BM_SETCHECK, state ? BST_CHECKED : BST_UNCHECKED, 0);
}

bool CheckBox::checked() const {
	return ::SendMessage(handle(), BM_GETCHECK, 0, 0) == BST_CHECKED;
}
}
