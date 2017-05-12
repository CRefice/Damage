#include "RadioButton.hpp"

namespace goo
{
void RadioButton::createControl() {
	createHandle("BUTTON", _text, BS_AUTORADIOBUTTON);
}
}
