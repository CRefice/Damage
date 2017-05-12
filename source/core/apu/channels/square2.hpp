#include "channel.hpp"

namespace core
{
class Square2 : public SoundChannel
{
public:
	void power(bool initializeLength = true);
	void run();

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

	void stepLength();
	void stepEnvelope();

private:
	uint11 frequency;
	int counter;
	uint3 phase;

	Envelope envelope;
	Length length;

	uint2 duty;
	bool dutyOutput;

	uint4 volume;
};
}
