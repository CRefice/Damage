#include <fstream>

#include "cartridge.hpp"

namespace core
{
void Cartridge::advance(uint cycles) {
	//Once a second
	if(secondCounter += cycles >= 1024 * 1024 * 4) {
		mbc3.second();
	}
}

void Cartridge::load(const std::string& path) {
	info.unserialize(path);
	rom.size = info.romSize;
	ram.size = info.ramSize;

	rom.data = new uint8[rom.size];
	std::ifstream romfile(path, std::ios::binary);
	romfile.read((char*)rom.data, rom.size);

	ram.data = new uint8[ram.size];
	if (info.battery) {
		auto dotPos = path.find_last_of('.');
		auto savePath = path.substr(0, dotPos) + ".sav";

		std::ifstream ramfile(savePath, std::ios::binary);
		ramfile.read((char*)ram.data, ram.size);
	}

	if (info.mapper == MapperType::MBC0) mapper = &mbc0;
	if (info.mapper == MapperType::MBC1) mapper = &mbc1;
	if (info.mapper == MapperType::MBC2) mapper = &mbc2;
	if (info.mapper == MapperType::MBC3) mapper = &mbc3;
}

void Cartridge::unload() {
	delete[] rom.data;
	delete[] ram.data;
	rom = {};
	ram = {};
}

void Cartridge::save(const std::string& path) {
	std::ofstream savefile(path, std::ios::binary);
	savefile.write((char*)ram.data, ram.size);
}

uint8 Cartridge::readIO(uint16 addr) {
	if (biosEnable && addr < 0x100) return biosData[addr];
	if (addr == 0xFF50) return 0xFF;
	return mapper->read(addr);
}

void Cartridge::writeIO(uint16 addr, uint8 val) {
	if (biosEnable && addr == 0xFF50) {
		biosEnable = false;
		return;
	}
	mapper->write(addr, val);
}

void Cartridge::Information::unserialize(const std::string& path) {
	std::ifstream file(path, std::ios::binary);

	battery = false;
	ram = false;
	rtc = false;
	rumble = false;

	file.seekg(0x147);
	switch (file.get()) {
	case 0x03:
		battery = true;
	case 0x02:
		ram = true;
	case 0x01:
		mapper = MapperType::MBC1;
		break;

	case 0x06:
		ram = true;
		battery = true;
	case 0x05:
		mapper = MapperType::MBC2;
		break;

	case 0x09:
		battery = true;
	case 0x08:
		ram = true;
	default: case 0x00:
		mapper = MapperType::MBC0;
		break;

	case 0x0D:
		battery = true;
	case 0x0C:
		ram = true;
	case 0x0B:
		mapper = MapperType::MMM;
		break;

	case 0x13:
		battery = true;
	case 0x12:
		ram = true;
	case 0x11:
		mapper = MapperType::MBC3;
		break;
	case 0x10:
		ram = true;
	case 0x0F:
		battery = true;
		rtc = true;
		mapper = MapperType::MBC3;
		break;

	case 0x1B:
		battery = true;
	case 0x1A:
		ram = true;
	case 0x19:
		mapper = MapperType::MBC5;
		break;
	case 0x1E:
		battery = true;
	case 0x1D:
		ram = true;
	case 0x1C:
		mapper = MapperType::MBC5;
		rumble = true;
		break;

	case 0x20:
		mapper = MapperType::MBC6;
		ram = true;
		battery = true;
		break;
	}

	file.seekg(0x148);
	romSize = 0x8000 << file.get();

	file.seekg(0x149);
	uint8 size = file.get();
	if (size > 0x00 && size < 0x05) ramSize = 0x800 << ((size - 1) << 1);
	else if (size == 0x05) ramSize = 0x10000;
	else ramSize = 0;
	if (mapper == MapperType::MBC2) ramSize = 512;
}

void Cartridge::power() {
	biosEnable = true;

	mbc0.connect(&rom, &ram);
	mbc1.connect(&rom, &ram);
	mbc2.connect(&rom, &ram);
	mbc3.connect(&rom, &ram);
	mbc0.power();
	mbc1.power();
	mbc2.power();
	mbc3.power();

	for (uint n = 0x0000; n < 0x8000; ++n) bus->map(n, this);
	for (uint n = 0xA000; n < 0xC000; ++n) bus->map(n, this);
	bus->map(0xFF50, this);
}

}
