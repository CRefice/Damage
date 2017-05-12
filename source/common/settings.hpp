#pragma once

class Settings
{
public:
	void load();
	void save();
			
	//Video Settings
	bool bilinearFiltering;
	bool integerScale;
};

extern Settings settings;