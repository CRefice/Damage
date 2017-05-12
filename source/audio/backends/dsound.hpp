#include <dsound.h>

#pragma comment(lib, "Dsound.lib")
#pragma comment(lib, "dxguid.lib")

#include "common\inttypes.hpp"
#include "audio\sound-stream.hpp"

namespace audio
{
class DSoundStream : public SoundStream
{
public:
	void init() final;
	void term() final;
	void power() final { clear(); }
	
	void play() final { dsb_b->Play(0, 0, DSBPLAY_LOOPING); }
	void stop() final { dsb_b->Stop(); }
	void clear();

	void sample(int16_t left, int16_t right) final;

private:
	LPDIRECTSOUND ds = nullptr;
	LPDIRECTSOUNDBUFFER dsb_p = nullptr;
	LPDIRECTSOUNDBUFFER dsb_b = nullptr;
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wfx;

	struct {
		uint rings = 0;
		uint latency = 0;

		uint32_t* buffer = nullptr;
		uint bufferoffset = 0;

		uint readring = 0;
		uint writering = 0;
		int distance = 0;
	} device;

	struct {
		HWND handle = nullptr;
		bool synchronize = true;
		uint frequency = 48000;
		uint latency = 120;
	} settings;
};
}
