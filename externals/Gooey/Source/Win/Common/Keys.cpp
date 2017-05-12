#include "Platform.hpp"

#include "Keys.hpp"

namespace goo
{
bool keyDown(Key key) {
	//TODO: give more information...?
  return ::GetAsyncKeyState(static_cast<uint8_t>(key)) != 0;
}

Point mousePos() {
  POINT retVal;
  ::GetCursorPos(&retVal);
  return { retVal.x, retVal.y };
}
}
