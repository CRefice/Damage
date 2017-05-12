#include <Gooey\Include\App.hpp>

#include "audio\sound-stream.hpp"

#include "input\input-common.hpp"
#include "input\control-emu.hpp"

#include "video\video-backend.hpp"

#include "emulator.hpp"

namespace emulator
{
core::System system;

static bool stop = true;
static bool loaded = false;
static std::string savePath;

void power() {
	system.power();
	system.setControls(&input::emuControls);
}

void load(const std::string& path) {
	if (loaded) system.save(savePath);
	system.load(path);
	loaded = true;

	//extract savepath
	const auto dotpos = path.find_last_of('.');
	const auto tilDot = path.substr(0, dotpos);
	savePath = tilDot + ".sav";
}

void unload() {
	if (loaded) system.save(savePath);
	system.unload();
}

void run() {
	stop = false;

	audio::stream->play();

	while(!stop) {
		input::poll();
		system.runFrame();
		video::backend->draw(system.screenBuffer(), 160, 144);
		goo::App::pollEvents();
	}
}
void pause() {
	audio::stream->stop();
	stop = true;
}
}
