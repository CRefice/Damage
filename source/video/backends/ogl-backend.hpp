#pragma once

#include "video\video-backend.hpp"

namespace goo
{
class GLWindow;
}

namespace video
{
class OGLBackend : public VideoBackend
{
public:
	uint32 videoColor(uint16 color) const final;

	void init(void* window) final;
	void draw(const uint16* buffer, uint width, uint height) final;
	void term() final;

private:
	uint VBO, VAO, EBO;
	uint texture;
	uint program;

	uint texWidth, texHeight;
	uint32_t* texData;
	goo::GLWindow* window;
};
}
