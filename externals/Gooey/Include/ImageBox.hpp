#pragma once

#include "Control.hpp"
#include "Image.hpp"

namespace goo
{
class ImageBox : public Control
{
public: 
	ImageBox() : Control(), _image() {}
	ImageBox(Image image) : _image(image), Control() {}
	ImageBox(Image image, const Point& pos, const Size& size) : _image(std::move(image)), Control(pos, size) {}

	Image image() const { return _image; }
	void setImage(Image image);

private:
	void createControl() override;

	Image _image;
};
}
