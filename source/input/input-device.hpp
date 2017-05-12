#pragma once

#include "common\inttypes.hpp"

#include <memory>
#include <string>
#include <vector>

namespace input
{
typedef double InputState;

class Input
{
public:
	virtual std::string name() const = 0;
	virtual InputState state() const = 0;
};

class Device
{
public:
	virtual ~Device() {}

	virtual std::string name() const = 0;

	virtual void poll() = 0;
	virtual Input* findInput(const std::string& name) = 0;
	
protected:
	std::vector<std::unique_ptr<Input>> inputs;
};

extern std::vector<std::shared_ptr<Device>> devices;
}
