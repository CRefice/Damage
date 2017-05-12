#pragma once

#include <cstdint>

namespace goo
{
struct Point
{
  inline Point() : x(0), y(0) {}
  inline Point(int x, int y) : x(x), y(y) {}

  int x, y;

  inline Point& operator+=(const Point& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
  inline Point& operator-=(const Point& rhs) {
	  x -= rhs.x;
	  y -= rhs.y;
	  return *this;
  }
};

inline Point operator+(Point lhs, const Point& rhs) {
	return lhs += rhs;
}
inline Point operator-(Point lhs, const Point& rhs) {
	return lhs -= rhs;
}

//lazy, I know, but... eh, whatever.
//remind me to implement this later.
typedef Point Size;

Point defaultPosition();
Size defaultSize();

struct Color
{
  uint8_t r, g, b;

  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  static Color red();
  static Color green();
  static Color blue();

  static Color black();
  static Color white();

  static Color control();
};
}
