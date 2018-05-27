#include <NumberFormat.h>

namespace cppsupport {
	std::string NumberFormat::ToHex(unsigned char c)
	{
		if (c == 0)
			return "00";
		else
		{
			char newString[3];

			unsigned char tmp((c & 0xf0) >> 4);
			if (tmp > 9)
				newString[0] = 'a' + (tmp - 10);
			else
				newString[0] = '0' + tmp;

			tmp = (c & 0x0f);
			if (tmp > 9)
				newString[1] = 'a' + (tmp - 10);
			else
				newString[1] = '0' + tmp;

			newString[2] = 0;

			return std::string(newString);
		}
	}
	std::string NumberFormat::ToHex(unsigned char const* c, int count, bool insertSpace)
	{
		std::string returnvalue;

		for (int i = 0; i < count; i++)
		{
			if (insertSpace && i > 0)
				returnvalue += " ";

			returnvalue += NumberFormat::ToHex(c[i]);
		}

		return returnvalue;
	}
	std::string NumberFormat::ToHex(signed char c)
	{
		std::string returnvalue;

		if (c < 0)
		{
			returnvalue += "-";
			returnvalue += NumberFormat::ToHex((unsigned char)(c * -1));
		}
		else
			returnvalue += NumberFormat::ToHex((unsigned char)c);

		return returnvalue;
	}
	std::string NumberFormat::ToHex(signed char const* c, int count, bool insertSpace)
	{
		std::string returnvalue;

		for (int i = 0; i < count; i++)
		{
			if (insertSpace && i > 0)
				returnvalue += " ";

			returnvalue += NumberFormat::ToHex(c[i]);
		}

		return returnvalue;
	}
	std::string NumberFormat::ToHex(char c, bool sign)
	{
		if (sign)
			return NumberFormat::ToHex((signed char)c);
		else
			return NumberFormat::ToHex((unsigned char)c);
	}
	std::string NumberFormat::ToHex(char const* c, int count, bool sign, bool insertSpace)
	{
		if (sign)
			return NumberFormat::ToHex((signed char*)c, count, insertSpace);
		else
			return NumberFormat::ToHex((unsigned char*)c, count, insertSpace);
	}

	static void hostToNetwork(char* c, int count)
	{
		static bool same(false);
		static bool checked(false);
		if (!checked)
		{
			uint16_t t(1);
			same = ((char*)&t)[1] == 1;
			checked = true;
		}

		if (!same)
		{
			for (int is = 0, ie = count - 1; is < ie; ie--,is++)
			{
				char tmp = c[ie];
				c[ie] = c[is];
				c[is] = tmp;
			}
		}
	}

	std::string NumberFormat::ToHex(uint64_t number)
	{
		char* c((char*)&number);
		hostToNetwork(c, sizeof(number));
		return NumberFormat::ToHex((unsigned char*)c, sizeof(number), false);
	}
	std::string NumberFormat::ToHex(int64_t number)
	{
		std::string rv;
		if (number < 0)
		{
			rv += "-";
			number *= -1;
		}
		
		char* c((char*)&number);
		hostToNetwork(c, sizeof(number));
		return rv + NumberFormat::ToHex((unsigned char*)c, sizeof(number), false);
	}
	std::string NumberFormat::ToHex(uint32_t number)
	{
		char* c((char*)&number);
		hostToNetwork(c, sizeof(number));
		return NumberFormat::ToHex((unsigned char*)c, sizeof(number), false);
	}

	std::string NumberFormat::ToHex(int32_t number)
	{
		std::string rv;
		if (number < 0)
		{
			rv += "-";
			number *= -1;
		}

		char* c((char*)&number);
		hostToNetwork(c, sizeof(number));
		return rv + NumberFormat::ToHex((unsigned char*)c, sizeof(number), false);
	}
}
