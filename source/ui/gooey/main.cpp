#include <Gooey/Include/App.hpp>

#include "emulator.hpp"

#include "audio\sound-stream.hpp"
#include "input\input-common.hpp"
#include "video\video-backend.hpp"

#include "main-window.hpp"

void main() {
	audio::init();
	video::init();
	input::init();

	MainWindow window;
	window.show();

	video::backend->init(&window);
	
	std::at_quick_exit([]() {
		audio::term();
		video::term();
		input::term();

		emulator::unload();
	});

	goo::App::run();
}