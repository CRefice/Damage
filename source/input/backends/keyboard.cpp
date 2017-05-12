#include "keyboard.hpp"

namespace input
{
namespace keyboard
{
using goo::Key;

static const struct
{
	const char* name;
	const Key key;
} namedKey[] = {
	{ "MouseL", Key::MouseL },
	{ "MouseR", Key::MouseR },
	{ "MouseM", Key::MouseM },

	{ "Backspace", Key::Backspace },
	{ "Tab", Key::Tab },
	{ "Enter", Key::Enter },
	{ "Return", Key::Return },
	{ "Shift", Key::Shift },
	{ "Control", Key::Control },
	{ "Alt", Key::Alt },
	{ "CapsLock", Key::CapsLock },
	{ "Esc", Key::Esc },
	{ "Space", Key::Space },
	{ "Delete", Key::Delete },

	{ "Left", Key::Left },
	{ "Up", Key::Up },
	{ "Right", Key::Right },
	{ "Down", Key::Down },

	{ "D0", Key::D0 },
	{ "D1", Key::D1 },
	{ "D2", Key::D2 },
	{ "D3", Key::D3 },
	{ "D4", Key::D4 },
	{ "D5", Key::D5 },
	{ "D6", Key::D6 },
	{ "D7", Key::D7 },
	{ "D8", Key::D8 },
	{ "D9", Key::D9 },

	{ "A", Key::A },
	{ "B", Key::B },
	{ "C", Key::C },
	{ "D", Key::D },
	{ "E", Key::E },
	{ "F", Key::F },
	{ "G", Key::G },
	{ "H", Key::H },
	{ "I", Key::I },
	{ "J", Key::J },
	{ "K", Key::K },
	{ "L", Key::L },
	{ "M", Key::M },
	{ "N", Key::N },
	{ "O", Key::O },
	{ "P", Key::P },
	{ "Q", Key::Q },
	{ "R", Key::R },
	{ "S", Key::S },
	{ "T", Key::T },
	{ "U", Key::U },
	{ "V", Key::V },
	{ "W", Key::W },
	{ "X", Key::X },
	{ "Y", Key::Y },
	{ "Z", Key::Z },

	{ "N0", Key::N0 },
	{ "N1", Key::N1 },
	{ "N2", Key::N2 },
	{ "N3", Key::N3 },
	{ "N4", Key::N4 },
	{ "N5", Key::N5 },
	{ "N6", Key::N6 },
	{ "N7", Key::N7 },
	{ "N8", Key::N8 },
	{ "N9", Key::N9 },

	{ "F1", Key::F1 },
	{ "F2", Key::F2 },
	{ "F3", Key::F3 },
	{ "F4", Key::F4 },
	{ "F5", Key::F5 },
	{ "F6", Key::F6 },
	{ "F7", Key::F7 },
	{ "F8", Key::F8 },
	{ "F9", Key::F9 },
	{ "F10", Key::F10 },
	{ "F11", Key::F11 },
	{ "F12", Key::F12 },
	{ "F13", Key::F13 },
	{ "F14", Key::F14 },
	{ "F15", Key::F15 },
	{ "F16", Key::F16 },
	{ "F17", Key::F17 },
	{ "F18", Key::F18 },
	{ "F19", Key::F19 },
	{ "F20", Key::F20 },
	{ "F21", Key::F21 },
	{ "F22", Key::F22 },
	{ "F23", Key::F23 },
	{ "F24", Key::F24 }
};

void init() {}
void enumDevices() {
	devices.push_back(std::make_shared<Keyboard>());
}
void term() {}

Keyboard::Keyboard() {
	for (uint n = 0; n < sizeof(namedKey) / sizeof(*namedKey); ++n) {
		inputs.push_back(std::make_unique<KeyInput>(n));
	}
}

void Keyboard::poll() {}

Input* Keyboard::findInput(const std::string& name) {
	for (const auto& input : inputs) {
		if (input->name() == name) return input.get();
	}
	return nullptr;
}

InputState KeyInput::state() const {
	return goo::keyDown(namedKey[index].key);
}

std::string KeyInput::name() const {
	return namedKey[index].name;
}
}
}
