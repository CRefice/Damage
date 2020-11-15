#include <Gooey\Include\App.hpp>
#include <Gooey\Include\FileDialog.hpp>

#include <algorithm>

#include "emulator.hpp"

#include "main-window.hpp"

using goo::OpenFileDialog;
 
MainWindow::MainWindow() {
	setText("Damage");
	setMenuBar(&menuBar);

	setClientArea({ 320, 288 });

	onClose.setHandler([](goo::CancelToken& cancel) {
		std::quick_exit(0);
	});

	onResize.setHandler([this](goo::Size size) {
		size = clientArea();
		const int ratio = std::min(size.x / 160, size.y / 144);
		const goo::Size finalSize = { 160 * ratio, 144 * ratio };
		const goo::Point offset = { (size.x - finalSize.x) / 2, (size.y - finalSize.y) / 2 };
		setViewport(offset, finalSize);
	});

	menuBar.loadItem.onClick.setHandler([]() {
		OpenFileDialog dialog("Open a ROM file", "GameBoy ROMs (*.gb)|*.gb");	
		if(dialog.show() == goo::DialogResult::OK) {
			emulator::load(dialog.fileName());
			emulator::power();
			emulator::run();
		}
	});

	menuBar.pauseItem.onClick.setHandler([this]() {
		if (!menuBar.pauseItem.checked()) {
			emulator::pause();
			menuBar.pauseItem.setChecked(true);
		}
		else {
			emulator::run();
			menuBar.pauseItem.setChecked(false);
		}
	});

	menuBar.resetItem.onClick.setHandler([]() {
			emulator::power();
			emulator::run();
	});
}
