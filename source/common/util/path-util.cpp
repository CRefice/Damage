#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#include "path-util.hpp"

std::string pathutil::configPath() {
#ifdef _WIN32
	char* name = new char[1000];
	::GetModuleFileName(NULL, name, 1000);
	std::string path(name);
	const auto pathPos = path.find_last_of('\\');
	return (path.substr(0, pathPos + 1) + "Damage.ini");

#else
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	std::string path(homedir);
	const auto pathPos = path.find_last_of('/');
	return (path.substr(0, pathPos + 1) + "Damage.ini");
#endif
}