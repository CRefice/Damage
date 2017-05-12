#include "channel.hpp"

namespace core
{
class Noise : public SoundChannel
{
public:
	void power(bool initializeLength = true);
	void run();

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

	void stepLength();
	void stepEnvelope();

private:
	uint getCounter() const;

	uint4 frequency;
	int counter;

	bool widthSelect;
	uint3 ratio;

	Length length;
	Envelope envelope;

	uint15 lfsr;
	uint4 volume;
};
}
