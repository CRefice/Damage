#pragma once

#include "Control.hpp"
#include "Event.hpp"

namespace goo
{
enum class SliderLayoutStyle {
	Horizontal, Vertical
};

class Slider : public Control
{
public:
	Slider() : Control(), _minValue(0), _maxValue(100) {}
	Slider(int minValue, int maxValue)
		: Control(), _minValue(minValue), _maxValue(maxValue) {}
	Slider(int minValue, int maxValue, const Point& pos, const Size& size)
		: Control(pos, size), _minValue(minValue), _maxValue(maxValue) {}

	Event<int> onValueChanged;

	int minValue() const { return _minValue; }
	int maxValue() const { return _maxValue; }

	int value() const;
	void setValue(int value);

private:
	void createControl() override;

	SliderLayoutStyle _layoutStyle;
	int _minValue, _maxValue;
	int _startVal;
};
}
