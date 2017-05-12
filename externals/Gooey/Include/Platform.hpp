#pragma once

#ifdef _WIN32
//Do not include other headers such as winsock.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//Do not define min and max macros, which are unneeded in C++
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>

//The WinApi defines a macro, MessageBox,
//which conflicts with our MessageBox namespace.
#undef MessageBox

namespace goo
{
using NativeControl = HWND;
using NativeFont = HFONT;
using NativeMenu = HMENU;
using NativeImage = HBITMAP;
using NativeLayout = void*; //WinApi doesn't use layouts
struct GLContext
{
  HDC hdc;
  HGLRC hglrc;
};

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

#elif defined(__linux__) //Should probably check for GTK in CMake instead.
#include <gtk/gtk.h>
	
namespace goo
{
using NativeControl = GtkWidget*;
using NativeFont = PangoFont*;
using NativeMenu = GtkMenuShell*;
using NativeImage = GtkPixbuf*;
using NativeLayout = GtkContainer*;
struct GLContext
{
  //HDC hdc;
  //HGLRC hglrc;
};

#else
#error "You're on an unsupported platform. Sorry!"

#endif
