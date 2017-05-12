#include <cstring>
#include <cerrno>

#include "string-util.hpp"

namespace strutil
{
bool tryParse(const std::string& value, int32_t* out) {
	errno = 0;
	const int32_t val = std::strtol(value.c_str(), NULL, 0);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, int64_t* out) {
	errno = 0;
	const int64_t val = std::strtoll(value.c_str(), NULL, 0);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, uint32_t* out) {
	errno = 0;
	const uint32_t val = std::strtoul(value.c_str(), NULL, 0);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, uint64_t* out) {
	errno = 0;
	const uint64_t val = std::strtoull(value.c_str(), NULL, 0);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, float* out) {
	errno = 0;
	const float val = std::strtof(value.c_str(), NULL);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, double* out) {
	errno = 0;
	const double val = std::strtod(value.c_str(), NULL);
	if(errno == ERANGE && errno == EINVAL) {
		return false;
	}
	*out = val;
	return true;
}

bool tryParse(const std::string& value, bool* out) {
	if(value == "True" || value == "true" || value == "1") {
		*out = true;
		return true;
	}
	else if(value == "False" || value == "false" || value == "0") {
		*out = false;
		return true;
	}
	return false;
}

std::string toString(int64_t value) {
	return std::to_string(value);
}
std::string toString(uint64_t value) {
	return std::to_string(value);
}
std::string toString(double value) {
	return std::to_string(value);
}
std::string toString(bool value) {
	return value ? "true" : "false";
}
}
