#pragma once

#include <string>
#include <vector>

#include "common\inttypes.hpp"

#include "input-common.hpp"
#include "input-device.hpp"

enum Buttons {
	A, B, Start, Select, Up, Down, Left, Right
};

namespace input
{
class Control
{
public:	
	Control(uint index) : index(index) {}

	void setReference(Input* reference) { input = reference; }

	std::string name();
	InputState state();

private:
	Input* input;
	const uint index;
};

class EmulatedControls
{
public:
	EmulatedControls();

	void setDevice(Device* device);
	bool buttonDown(Buttons button);

private:
	std::vector<Control> controls;
};

extern EmulatedControls emuControls;
}
