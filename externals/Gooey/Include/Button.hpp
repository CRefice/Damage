#pragma once

#include "ButtonBase.hpp"

namespace goo
{
class Button : public ButtonBase
{
public:
	Button() : ButtonBase() {}
	Button(std::string text) : ButtonBase(std::move(text)) {}
	Button(std::string text, const Point& pos, const Size& size) : ButtonBase(std::move(text), pos, size) {}

private:
	void createControl() override;
};
}
