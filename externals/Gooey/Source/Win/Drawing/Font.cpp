#include "Font.hpp"

namespace goo
{
Font::Font(std::string typeface, int size) : _typeface(std::move(typeface)), _size(size) {
	_handle = ::CreateFont(_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, _typeface.c_str());
}

Font defaultFont() {
	//Don't know why WinApi doesn't define a macro for this...
	return Font("Microsoft Sans Serif", 14);
}
}
