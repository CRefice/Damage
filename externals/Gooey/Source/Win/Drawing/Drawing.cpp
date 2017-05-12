#include "Platform.hpp"

#include "Drawing.hpp"

namespace goo
{
Point defaultPosition() {
	return{ CW_USEDEFAULT, CW_USEDEFAULT };
}
Point defaultSize() {
	return{ CW_USEDEFAULT, CW_USEDEFAULT };
}

Color Color::red() { return Color(0xFF, 0, 0); }
Color Color::green() { return Color(0, 0xFF, 0); }
Color Color::blue() { return Color(0, 0, 0xFF); }
Color Color::white() { return Color(0xFF, 0xFF, 0xFF); }
Color Color::black() { return Color(0, 0, 0); }
Color Color::control() { return Color(240, 240, 240); }
}
