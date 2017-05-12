#pragma once

#include <cstdint>
#include <string>

namespace strutil
{
bool tryParse(const std::string& value, int32_t* out);
bool tryParse(const std::string& value, int64_t* out);
bool tryParse(const std::string& value, uint32_t* out);
bool tryParse(const std::string& value, uint64_t* out);
bool tryParse(const std::string& value, float* out);
bool tryParse(const std::string& value, double* out);
bool tryParse(const std::string& value, bool* out);

std::string toString(int64_t value);
std::string toString(uint64_t value);
std::string toString(double value);
std::string toString(bool value);
}
