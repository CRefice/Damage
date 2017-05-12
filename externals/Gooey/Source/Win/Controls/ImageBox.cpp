#include "ImageBox.hpp"

namespace goo
{
void ImageBox::createControl() {
	createHandle("STATIC", "", SS_BITMAP);
	setImage(_image);
}

void ImageBox::setImage(Image image) {
	::SendMessage(handle(), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(HBITMAP)(image.handle()));
	_image = image;
}
}
