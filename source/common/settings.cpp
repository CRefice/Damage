#include "common\util\inifile.hpp"
#include "common\util\path-util.hpp"

#include "settings.hpp"

Settings settings;

void Settings::load()
{
	IniFile ini;
	ini.load(pathutil::configPath());
	auto& sec = ini.section("VideoSettings");
	sec.get("BilinearFiltering", &bilinearFiltering, false);
	sec.get("IntegerScale", &integerScale, true);
}

void Settings::save()
{
	IniFile ini;
	auto& sec = ini.section("VideoSettings");
	sec.set("BilinearFiltering", bilinearFiltering);
	sec.set("IntegerScale", integerScale);
	ini.save(pathutil::configPath());
}