#include "ComboBox.hpp"
#include "Button.hpp"
#include "Slider.hpp"
#include "Window.hpp"

#include "Platform.hpp"

namespace goo
{
template<typename T>
static T* ptrFromHandle(HWND handle) {
	//dynamic_cast should work... research
	return static_cast<T*>(::GetProp(handle, "PROP_CONTROL"));
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Window* sender = ptrFromHandle<Window>(hwnd);

	switch (msg) {
	case WM_DESTROY: {
		if (sender) sender->onDestroy();
		return 0;
	}

	case WM_CLOSE: {
		if (!sender) return 0;

		CancelToken token;
		sender->onClose(token);
		if (!token.cancel) sender->close();
		return 0;
	}

	case WM_COMMAND: {
		switch (HIWORD(wparam)) {
		case BN_CLICKED: {
			ButtonBase* clicked = ptrFromHandle<ButtonBase>((HWND)lparam);
			if (clicked) clicked->onClick();

			else if (sender->menuBar() != nullptr) {
				sender->menuBar()->itemFromId(LOWORD(wparam))->onClick();
			}
			break;
		}
		case CBN_SELCHANGE: {
			ComboBox* box = ptrFromHandle<ComboBox>((HWND)lparam);
			if(box) box->onSelectionChanged(box->selectedIndex());
			break;
		}
		}
		return 0;
	}
	case WM_SIZE: {
		if (sender) sender->onResize({ LOWORD(lparam), HIWORD(lparam) });
		return 0;
	}
	case WM_HSCROLL:
	case WM_VSCROLL: {
		switch(LOWORD(wparam)) {
			case TB_BOTTOM:
			case TB_ENDTRACK:
			case TB_LINEDOWN:
			case TB_LINEUP:
			case TB_PAGEDOWN:
			case TB_PAGEUP:
			case TB_THUMBPOSITION:
			case TB_THUMBTRACK:
			case TB_TOP:
			{
				Slider* slider = ptrFromHandle<Slider>((HWND)lparam);
				slider->onValueChanged(slider->value());
				break;
			}
		}
		return 0;
	}
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
}
}
