#include <exception>

#include "Window.hpp"

namespace goo
{
void Window::createControl() {
	WNDCLASSEX wclass;
	::ZeroMemory(&wclass, sizeof(wclass));
	wclass.cbSize = sizeof(wclass);
	wclass.lpfnWndProc = wndProc;
	wclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wclass.hInstance = ::GetModuleHandle(NULL);
	wclass.hbrBackground = ::GetSysColorBrush(COLOR_3DFACE);
	wclass.lpszClassName = _text.c_str();

	::RegisterClassEx(&wclass);

	long wndStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | CS_OWNDC;
	long exStyle = 0;

	switch (_borderStyle) {
	case BorderStyle::Sizeable:
		wndStyle |= WS_THICKFRAME;
		break;

	case BorderStyle::Sunken:
		exStyle |= WS_EX_CLIENTEDGE;
		break;
	}
	switch (_titleButtons) {
	case StatusButtons::Maximize:
		wndStyle |= WS_MAXIMIZEBOX;
		break;

	case StatusButtons::Minimize:
		wndStyle |= WS_MINIMIZEBOX;
		break;

	case StatusButtons::Both:
		wndStyle |= (WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	}

	createHandle(_text.c_str(), _text, wndStyle, exStyle);
	if (_menuBar) ::SetMenu(handle(), _menuBar->handle());

	createChildren();
}

void Window::close() {
	::DestroyWindow(handle());
}

void Window::setMenuBar(MenuBar* menubar) {
	::SetMenu(handle(), menubar->handle());
	_menuBar = menubar;
}

void Window::setText(std::string text) {
	::SetWindowText(handle(), text.c_str());
	_text = std::move(text);
}

void Window::setBorderStyle(BorderStyle style) {
	//TODO
	_borderStyle = style;
}

void Window::setTitleBarButtons(StatusButtons buttons) {
	//TODO
	_titleButtons = buttons;
}

void Window::setClientArea(const Size& area) {
	RECT rect;
	::SetRect(&rect, 0, 0, area.x + 1, area.y + 1);

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | CS_OWNDC, menuBar() != nullptr);
	Control::setSize({ rect.right - rect.left - 1, rect.bottom - rect.top - 1 });
}

Size Window::clientArea() {
	RECT rect;
	::GetClientRect(handle(), &rect);
	return Size(rect.right - rect.left, rect.bottom - rect.top);
}
}
