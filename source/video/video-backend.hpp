#pragma once

#include <memory>
#include "common\inttypes.hpp"

namespace video
{
void init();
void term();

class VideoBackend
{
public:
	virtual ~VideoBackend() { term(); }
	
	bool cgb() const { return isCgb; }
	void setCgb(bool state) { isCgb = state; }

	virtual uint32 videoColor(uint16 color) const = 0;

	virtual void init(void*) = 0;
	virtual void draw(const uint16* buffer, uint width, uint height) = 0;
	virtual void term() {};

private:
	bool isCgb;
};

extern std::unique_ptr<VideoBackend> backend;
}
