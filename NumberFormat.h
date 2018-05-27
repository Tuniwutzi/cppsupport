#pragma once

#include <string>

namespace cppsupport
{
	class NumberFormat
	{
	public:
		static std::string ToHex(unsigned char c);
		static std::string ToHex(unsigned char const* c, int count, bool insertSpace = false);
		static std::string ToHex(signed char c);
		static std::string ToHex(signed char const* c, int count, bool insertSpace = true); //Insert space hier true, da negative hex-Zahlen sonst irgendwo dazwischen auftauchen
		static std::string ToHex(char c, bool sign = true);
		static std::string ToHex(char const* c, int count, bool sign = true, bool insertSpace = true); //Insert space hier true, da negative hex-Zahlen sonst irgendwo dazwischen auftauchen
		static std::string ToHex(uint64_t number);
		static std::string ToHex(int64_t number);
		static std::string ToHex(uint32_t number);
		static std::string ToHex(int32_t number);
	};
}
