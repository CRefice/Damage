#pragma once

#include <string>

#include "memory\memory.hpp"
#include "mapper\mapper.hpp"

#include "mapper\mbc0.hpp"
#include "mapper\mbc1.hpp"
#include "mapper\mbc2.hpp"
#include "mapper\mbc3.hpp"

namespace core
{
class Cartridge : public MMIO
{
public:
	void power();
	void advance(uint cycles); //Needed for timing

	void load(const std::string& path);

	void save(const std::string& path);
	void unload();

private:
	MBC0 mbc0;
	MBC1 mbc1;
	MBC2 mbc2;
	MBC3 mbc3;

	uint8 readIO(uint16 addr) final;
	void writeIO(uint16 addr, uint8 val) final;

	enum class MapperType
	{
		MBC0,
		MBC1,
		MBC2,
		MMM,
		MBC3,
		MBC5,
		MBC6
	};

	struct Information
	{
		void unserialize(const std::string& file);

		std::string title;
		MapperType mapper;
		uint romSize, ramSize;

		bool ram;
		bool battery;
		bool rtc;
		bool rumble;
	} info;

	MemBlock rom, ram;

	Mapper* mapper;
	bool biosEnable;
	#include "bootrom.inl"

	uint secondCounter;
};
}
