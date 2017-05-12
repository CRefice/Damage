#include "Button.hpp"

namespace goo
{
void Button::createControl() {
	createHandle("BUTTON", text(), BS_PUSHBUTTON);
}
}
