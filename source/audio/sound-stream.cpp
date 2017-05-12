#include "backends\dsound.hpp"

#include "sound-stream.hpp"

namespace audio
{
std::unique_ptr<SoundStream> stream;

void init() {
	stream = std::make_unique<DSoundStream>();
	stream->init();
}

void term() {
	stream->term();
}

void sample(int16_t left, int16_t right) {
	auto& mixer = stream->mixer();
	mixer.sample(left / 32768.0, right / 32768.0);
}
}