#include <functional>

#include "Platform.hpp"

#include "App.hpp"

namespace goo
{
	void App::run() {
		MSG msg;
		while (::GetMessage(&msg, NULL, 0, 0) != 0) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	void App::quit() {
		::PostQuitMessage(0);
	}

	void App::pollEvents() {
		MSG Msg;
		while (::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
	}
}

#pragma comment(linker, "\"/manifestdependency:type='win32' \
						name='Microsoft.Windows.Common-Controls' \
						version='6.0.0.0' \
						processorArchitecture='*' \
						publicKeyToken='6595b64144ccf1df' language='*'\"")
