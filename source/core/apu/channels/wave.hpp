#include "channel.hpp"

namespace core
{
class Wave : public SoundChannel
{
public:
	void power(bool initializeLength = true);
	void run();

	void resetWave() { for (auto& n : pattern) n = 0; }

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

	void stepLength();

private:
	uint4 getPattern(uint5 offset) const;

	bool dacEnable;

	uint11 frequency;
	int counter;

	Length length;

	uint8 pattern[16];
	uint5 patternOffset;
	uint4 patternSample;
	bool patternHold;

	uint2 volume;
};
}
