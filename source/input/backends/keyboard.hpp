#pragma once

#include <Gooey\Include\Keys.hpp>

#include <memory>
#include <vector>

#include "input\input-device.hpp"

namespace input
{
namespace keyboard
{
void init();
void enumDevices();
void term();

class Keyboard : public Device
{
public:
	Keyboard();

	std::string name() const final { return "Keyboard"; }

	void poll() final;
	Input* findInput(const std::string& name) final;
};

class KeyInput : public Input
{
public:
	KeyInput(uint index) : index(index) {}

	InputState state() const final;
	std::string name() const final;

private:
	uint index;
};
}
}
