#pragma once

#include <string>

#include "Handle.hpp"

namespace goo
{
class Image
{
public:
	Image() : _handle() {}
	explicit Image(const std::string& filePath);
	explicit Image(ImageHandle handle) : _handle(handle) {}	

	ImageHandle& handle() { return _handle; }
	const ImageHandle& handle() const { return _handle; }

private:
	ImageHandle _handle;
};
}
