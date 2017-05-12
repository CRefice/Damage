#pragma once

#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput9_1_0.lib")
#pragma comment(lib, "dxguid.lib")

#include <memory>
#include <vector>

#include "input\input-device.hpp"

namespace input
{
namespace xinput
{
void init();
void enumDevices();
void term();

class Gamepad : public Device
{
public:
	Gamepad(uint index);

	std::string name() const final;

	void poll() final;
	Input* findInput(const std::string& name) final;

private:
	const uint index;
	XINPUT_STATE gpstate;
};

class Button : public Input
{
public:
	Button(uint index, const unsigned short& buttons) : index(index), buttons(buttons) {}
	InputState state() const final;
	std::string name() const final;

private:
	const unsigned short& buttons;
	const uint index;
};

class Axis : public Input
{
public:
	Axis(int index, const short& axis, short range) : index(index), axis(axis), range(range) {}

	InputState state() const final;
	std::string name() const final;

private:
	const short& axis;
	const uint index;
	const short range;
};

class Trigger : public Input
{
public:
	enum { LT = 0, RT = 1 };
	Trigger(uint index, const unsigned char& trigger) : index(index), trigger(trigger) {}

	InputState state() const final;
	std::string name() const final;

private:
	const unsigned char& trigger;
	const uint index;
};
}
}
