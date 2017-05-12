#pragma once

#include <cstdint>

#include <string>
#include <list>
#include <map>
#include <vector>

class IniFile
{
public:
	class Section
	{
	public:
		Section(const std::string& name) : name(name) {}

		void set(const std::string& key, const std::string& value);
		void set(const std::string& key, int64_t value);
		void set(const std::string& key, uint64_t value);
		void set(const std::string& key, double value);
		void set(const std::string& key, bool value);

		bool get(const std::string& key, std::string* out, const std::string& defaultVal = "");
		bool get(const std::string& key, int32_t* out, int32_t defaultVal = 0);
		bool get(const std::string& key, int64_t* out, int64_t defaultVal = 0);
		bool get(const std::string& key, uint32_t* out, uint32_t defaultVal = 0);
		bool get(const std::string& key, uint64_t* out, uint64_t defaultVal = 0);
		bool get(const std::string& key, float* out, float defaultVal = 0.0f);
		bool get(const std::string& key, double* out, double defaultVal = 0.0);
		bool get(const std::string& key, bool* out, bool defaultVal = false);

	private:
		const std::string name;
		std::map<std::string, std::string> lines;
		std::vector<std::string> orderedKeys;

		friend class IniFile;
	};

	bool load(const std::string& path);
	void save(const std::string& path);

	Section& section(const std::string& name);

private:
	std::list<Section> sections;
};
