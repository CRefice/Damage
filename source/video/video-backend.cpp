#pragma once

#include "backends\ogl-backend.hpp"

#include "video-backend.hpp"

namespace video
{
std::unique_ptr<VideoBackend> backend;

void init() {
#ifdef _WIN32
	backend = std::make_unique<OGLBackend>();
#endif
}

void term() {}
}
