#include <memory>
#include <vector>

#include "common\util\inifile.hpp"
#include "common\util\path-util.hpp"

#include "backends\xinput.hpp"
#include "backends\keyboard.hpp"

#include "input-device.hpp"
#include "control-emu.hpp"

#include "input-common.hpp"

namespace input
{
void init() {
	xinput::init();
	keyboard::init();

	enumDevices();

	IniFile inifile;
	if (!inifile.load(pathutil::configPath())) {
		emuControls.setDevice(devices.front().get());
		return;
	}

	IniFile::Section& section = inifile.section("Controls");
	std::string deviceString;
	section.get("Device", &deviceString);
	Device* device = nullptr;
	for(auto& dev : devices) {
		if(dev->name() == deviceString) {
			emuControls.setDevice(dev.get());
			break;
		}
	}
}

void enumDevices() {
	devices.clear();

	xinput::enumDevices();
	keyboard::enumDevices();
}

void term() {
	xinput::term();
	keyboard::term();
}

void poll() {
	for(auto& dev : devices) {
		dev->poll();
	}
}
}
