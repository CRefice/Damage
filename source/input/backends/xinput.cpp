#include <algorithm>

#include "xinput.hpp"

namespace input
{
namespace xinput
{
static const struct
{
	const char* const name;
	const WORD bitmask;
} namedButtons[] = {
	{ "Button A", XINPUT_GAMEPAD_A },
	{ "Button B", XINPUT_GAMEPAD_B },
	{ "Button X", XINPUT_GAMEPAD_X },
	{ "Button Y", XINPUT_GAMEPAD_Y },
	{ "Pad Up", XINPUT_GAMEPAD_DPAD_UP },
	{ "Pad Down", XINPUT_GAMEPAD_DPAD_DOWN },
	{ "Pad Left", XINPUT_GAMEPAD_DPAD_LEFT },
	{ "Pad Right", XINPUT_GAMEPAD_DPAD_RIGHT },
	{ "Start", XINPUT_GAMEPAD_START },
	{ "Back", XINPUT_GAMEPAD_BACK },
	{ "Shoulder L", XINPUT_GAMEPAD_LEFT_SHOULDER },
	{ "Shoulder R", XINPUT_GAMEPAD_RIGHT_SHOULDER },
	{ "Thumb L", XINPUT_GAMEPAD_LEFT_THUMB },
	{ "Thumb R", XINPUT_GAMEPAD_RIGHT_THUMB }
};

static const char* const namedAxes[] = {
	"Left X", "Left Y", "Right X", "Right Y"
};

static const char* const namedTriggers[] = {
	"Trigger L", "Trigger R"
};

static const double deadZone = 0.1;

void init() {}
void term() {}

void enumDevices() {
	XINPUT_STATE state;

	//Check if controllers are connected
	for(uint n = 0; n < XUSER_MAX_COUNT; ++n) {
		::ZeroMemory(&state, sizeof(state));
		auto result = XInputGetState(n, &state);
		if(result == ERROR_SUCCESS) {
			devices.push_back(std::make_shared<Gamepad>(n));
		}
	}
}

Gamepad::Gamepad(uint index) : index(index) {
	for (auto n = 0; n < sizeof(namedButtons) / sizeof(*namedButtons); ++n) {
		inputs.push_back(std::make_unique<Button>(n, gpstate.Gamepad.wButtons));
	}

	for (auto n = 0; n < sizeof(namedAxes) / sizeof(*namedAxes); ++n) {
		const SHORT& ax = (&(gpstate.Gamepad.sThumbLX))[n];
		inputs.push_back(std::make_unique<Axis>(n, ax, 32767));
		inputs.push_back(std::make_unique<Axis>(n, ax, -32768));
	}

	for (auto n = 0; n < sizeof(namedTriggers) / sizeof(*namedTriggers); ++n) {
		BYTE trigger = *(&(gpstate.Gamepad.bLeftTrigger) + n);
		inputs.push_back(std::make_unique<Trigger>(n, trigger));
	}
}

std::string Gamepad::name() const {
	return "Gamepad/" + std::to_string(index);
}

void Gamepad::poll() {
	::XInputGetState(index, &gpstate);
}

Input* Gamepad::findInput(const std::string& name) {
	for (const auto& input : inputs) {
		if (input->name() == name) return input.get();
	}
	return nullptr;
}

InputState Button::state() const {
	return (buttons & namedButtons[index].bitmask) > 0;
}

std::string Button::name() const {
	return namedButtons[index].name;
}

InputState Axis::state() const {
	return std::max(0.0, ((InputState)axis / range) - deadZone);
}

std::string Axis::name() const {
	return std::string(namedAxes[index]) + (range < 0 ? '-' : '+');
}

InputState Trigger::state() const {
	return (InputState)trigger / 255;
}

std::string Trigger::name() const {
	return namedTriggers[index];
}
}
}
