#pragma once

#include "common\util\inifile.hpp"
#include "common\util\path-util.hpp"

#include "control-emu.hpp"

namespace input
{
EmulatedControls emuControls;

static const char* namedControls[] = {
	"Button A", "Button B", "Button Start", "Button Select",
	"DPad Up", "DPad Down", "DPad Left", "DPad Right"
};

std::string Control::name() {
	return namedControls[index];
}

InputState Control::state() {
	if (input == nullptr) return InputState();
	else return input->state();
}

EmulatedControls::EmulatedControls() {
	for(uint n = 0; n < sizeof(namedControls) / sizeof(*namedControls); ++n) {
		controls.emplace_back(n);
	}
}

void EmulatedControls::setDevice(Device* device) {
	IniFile inifile;
	if (!inifile.load(pathutil::configPath())) {
		controls[0].setReference(device->findInput("X"));
		controls[1].setReference(device->findInput("C"));
		controls[2].setReference(device->findInput("Enter"));
		controls[3].setReference(device->findInput("Backspace"));
		controls[4].setReference(device->findInput("Up"));
		controls[5].setReference(device->findInput("Down"));
		controls[6].setReference(device->findInput("Left"));
		controls[7].setReference(device->findInput("Right"));
		return;
	}
	IniFile::Section& section = inifile.section("Controls");

	for(uint n = 0; n < sizeof(namedControls) / sizeof(*namedControls); ++n) {
		std::string inputName;
		section.get(namedControls[n], &inputName);
		controls[n].setReference(device->findInput(inputName));
	}
}

bool EmulatedControls::buttonDown(Buttons button) {
	return controls[int(button)].state() != 0;
}
}
