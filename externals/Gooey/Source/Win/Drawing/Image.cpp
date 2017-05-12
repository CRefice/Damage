#include <algorithm>
//#include <wincodec.h> Soon...

#include "Image.hpp"

namespace goo
{
Image::Image(const std::string& filePath) {
	//Find file extension and lowercase it.
	std::string ext = (filePath.substr(filePath.find_last_of('.') + 1));
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

	//BMP and ICO files are natively supported, so check for those first
	if (ext == "bmp")
	{
		_handle = (HBITMAP)LoadImage(NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		return;
	}
	if (ext == "ico")
	{
		_handle = (HBITMAP)LoadImage(NULL, filePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		return;
	}
	
	//If it isn't one of those, we're gonna have to delve into the cesspool that is COM...
	//TODO: Coming soon...
}
}
