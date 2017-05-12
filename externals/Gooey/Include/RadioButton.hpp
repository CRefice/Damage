#pragma once

#include "ButtonBase.hpp"

namespace goo
{
class RadioButton : public ButtonBase
{
public:
	RadioButton() : ButtonBase() {}
	RadioButton(std::string text) : ButtonBase(std::move(text)) {}
	RadioButton(std::string text, const Point& pos, const Size& size) : ButtonBase(std::move(text), pos, size) {}

private:
	void createControl() override;
};
}
