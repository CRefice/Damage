#pragma once

#include <string>

#include "Handle.hpp"

namespace goo
{
enum class FontStyle
{
  Bold, Italic, Underline, Strikethrough
};

class Font
{
public:
  Font(std::string typeface, int size);
  
  FontHandle& handle() { return _handle; }
  const FontHandle& handle() const { return _handle; }

	std::string typeface() { return _typeface; }
	std::string setTypeface();
  
	int size() { return _size; }
	void setSize(int size);

private:
  std::string _typeface;
  int _size;
  
  FontHandle _handle;
};
Font defaultFont();
}
