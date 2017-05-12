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
	inifile.load(pathutil::configPath());
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
