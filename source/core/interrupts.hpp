#pragma once

namespace core
{
enum class Interrupt
{
	VBlank, LCD, Timer, Serial, Joypad
};

class InterruptHandler
{
public:
	virtual void handle(Interrupt i) = 0;
};

class InterruptRequester
{
public:
	void connect(InterruptHandler* handler);
	void raise(Interrupt i);

private:
	InterruptHandler* irqHandler;
};
}
