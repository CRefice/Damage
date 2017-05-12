#include "channel.hpp"

namespace core
{
class Square1 : public SoundChannel
{
public:
	void power(bool initializeLength = true);
	void run();

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

	void stepLength();
	void stepEnvelope();
	void stepSweep();
	void updateSweep(bool save);

private:
	uint11 frequency;
	uint counter;
	uint3 phase;

	Length length;
	Envelope envelope;
	Sweep sweep;

	uint2 duty;
	bool dutyOutput;

	uint4 volume;
};
}
