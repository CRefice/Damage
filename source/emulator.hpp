#pragma once

#include <string>

#include "core\system.hpp"

namespace emulator
{
void power();

void load(const std::string& path);
void unload();

void run();
void pause();
}
