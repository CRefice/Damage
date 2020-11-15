#include <Gooey/Include/FileDialog.hpp>

#include "menu-bar.hpp"

MainMenuBar::MainMenuBar() {
	appendItem(fileMenu);
	fileMenu.appendItem(loadItem);

	appendItem(emuMenu);
	emuMenu.appendItem(pauseItem);
	emuMenu.appendItem(resetItem);

	//appendItem(optionsMenu);
}
