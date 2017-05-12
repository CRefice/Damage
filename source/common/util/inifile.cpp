#include <fstream>

#include "string-util.hpp"

#include "inifile.hpp"

void IniFile::Section::set(const std::string& key, const std::string& value) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		it->second = value;
	}
	else {
		orderedKeys.push_back(key);
		lines[key] = value;
	}
}
void IniFile::Section::set(const std::string& key, int64_t value) {
	set(key, strutil::toString(value));
}
void IniFile::Section::set(const std::string& key, uint64_t value) {
	set(key, strutil::toString(value));
}
void IniFile::Section::set(const std::string& key, double value) {
	set(key, strutil::toString(value));
}
void IniFile::Section::set(const std::string& key, bool value) {
	set(key, strutil::toString(value));
}

bool IniFile::Section::get(const std::string& key, std::string* out, const std::string& defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		*out = it->second;
		return true;
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, int32_t* out, int32_t defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, int64_t* out, int64_t defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, uint32_t* out, uint32_t defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, uint64_t* out, uint64_t defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, float* out, float defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, double* out, double defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}
bool IniFile::Section::get(const std::string& key, bool* out, bool defaultVal) {
	auto it = lines.find(key);
	if(it != lines.end()) {
		if(strutil::tryParse(key, out)) {
			return true;
		}
	}
	*out = defaultVal;
	return false;
}

bool IniFile::load(const std::string& path) {
	std::ifstream file(path, std::ios::beg);
	if (file.fail()) return false;

	std::string line;
	Section* currentSection = nullptr;

	while (std::getline(file, line)) {
		//Discard comments
		if (line.front() == ';' || line.front() == '#') continue;

		//New section
		if (line.front() == '[') {
			const auto endpos = line.find_first_of(']') - 1;
			currentSection = &section(line.substr(1, endpos));
			continue;
		}

		if (currentSection == nullptr) return false;
		const auto equalsPos = line.find_first_of('=');
		const auto key = line.substr(0, equalsPos);
		const auto val = line.substr(equalsPos + 1);

		currentSection->set(key, val);
	}
	file.close();
	return true;
}

void IniFile::save(const std::string& path) {
	std::ofstream file(path, std::ios::trunc | std::ios::beg);
	if (file.fail()) return;

	for (const auto& section : sections) {
		file << '[' << section.name << ']' << '\n';
		for (const auto& key : section.orderedKeys) {
			std::string val = section.lines.at(key);
			file << key << '=' << val << '\n';
		}
	}
	file.close();
}

IniFile::Section& IniFile::section(const std::string& name) {
	for (auto& s : sections) {
		if (s.name == name) return s;
	}

	sections.push_back(Section(name));
	return sections.back();
}
