#include "Slider.hpp"

namespace goo
{
void Slider::createControl() {
	long style = _layoutStyle == SliderLayoutStyle::Vertical ? TBS_VERT : TBS_HORZ;
	createHandle(TRACKBAR_CLASS, "", style);
}

int Slider::value() const {
	return ::SendMessage(handle(), TBM_GETPOS, 0, 0); 
}

void Slider::setValue(int value) {
	::SendMessage(handle(), TBM_GETPOS, (WPARAM)TRUE, (LPARAM)value);
	_startVal = value;
}
}
