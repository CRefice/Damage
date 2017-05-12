#include "Platform.hpp"
#include <gl\GL.h>

#include "GLWindow.hpp"

//Little helper function to get extensions
//I shouldn't need this really... should I?
static void* getAnyGLFuncAddress(const char* name) {
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}
	return p;
}

namespace goo
{
void GLWindow::createControl() {
	Window::createControl();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	_context.hdc = ::GetDC(handle());

	const int format = ::ChoosePixelFormat(_context.hdc, &pfd);
	::SetPixelFormat(_context.hdc, format, &pfd);

	_context.hglrc = ::wglCreateContext(_context.hdc);
	::wglMakeCurrent(_context.hdc, _context.hglrc);

	setViewport({ 0, 0 }, clientArea());
}

void GLWindow::swapBuffers() {
	::SwapBuffers(_context.hdc);
}

void GLWindow::setViewport(const Point& point, const Size& size) {
	::glViewport(point.x, point.y, size.x, size.y);
}

void GLWindow::setFullscreen(bool enable) {
//TODO
}

void GLWindow::setVsync(bool enable) {
	typedef void(*VSyncFunc)(int);
	VSyncFunc func = (VSyncFunc)getAnyGLFuncAddress("wglSwapIntervalEXT");
	func(enable);
}
}
