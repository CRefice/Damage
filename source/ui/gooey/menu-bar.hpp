#pragma once

#include <Gooey/Include/Menu.hpp>

class MainMenuBar : public goo::MenuBar
{
public:
	MainMenuBar();

	goo::PopupMenuItem fileMenu{ "File" };
	goo::TextItem loadItem{ "Open..." };

	goo::PopupMenuItem emuMenu{ "Emulation" };
	goo::TextItem pauseItem{ "Pause" };
	goo::TextItem resetItem{ "Reset" };

	goo::PopupMenuItem optionsMenu{ "Options" };
};
