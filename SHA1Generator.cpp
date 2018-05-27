#include <SHA1Generator.h>

#include <NumberFormat.h>

#include <sha1.h>

 //htonl
#ifdef _WINDOWS
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

using namespace std;

namespace cppsupport
{
	SHA1Generator::SHA1Generator()
	{
	}
	//TODO: Gibt einen falschen Hash fuer leeren std::string aus
	vector<char> SHA1Generator::generate(char const* in, unsigned int length) const
	{
		static unsigned int const messageDigestLength = sizeof(unsigned int) * 5;

		vector<char> returnvalue;
		returnvalue.resize(messageDigestLength);

		SHA1Private sha;

		char* hash = returnvalue.data();
		unsigned int* messageDigest = (unsigned int*)hash;

		sha.Reset();
		sha.Input(in, length);

		if (sha.Result(messageDigest))
		{
			for (int i = 0; i < (messageDigestLength / sizeof(unsigned int)); i++)
				messageDigest[i] = htonl(messageDigest[i]);

			return returnvalue;
		}
		else
			throw std::runtime_error("Could not generate SHA1 Hash");
	}
	vector<char> SHA1Generator::generate(vector<char> const& in) const
	{
		return this->generate(in.data(), (unsigned int)in.size());
	}
	vector<char> SHA1Generator::generate(std::string const& in) const
	{
		return this->generate(in.data(), (unsigned int)in.size());
	}
	
	//TODO: Richtiger Hash fuer leeren std::string per Sonderfall angegeben; sonst gleiches Problem wie bei .generate
	std::string SHA1Generator::generateChecksum(char const* in, unsigned int length, ChecksumFormats type) const
	{
		if (length == 0)
			return "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709";

		unsigned int messageDigest[5];

		SHA1Private sha;
		sha.Reset();
		sha.Input(in, length);

		if (sha.Result(messageDigest))
		{
			std::string result;

			if (type == ChecksumFormats::Hex)
			{
				for (int i = 0; i < sizeof(messageDigest) / sizeof(unsigned int); i++)
					result += NumberFormat::ToHex(messageDigest[i]);
			}
			else
				throw std::runtime_error("Unknown ChecksumFormat");

			return result;
		}
		else
			throw std::runtime_error("Could not generate SHA1 Hash");
	}
	std::string SHA1Generator::generateChecksum(vector<char> const& in, ChecksumFormats type) const
	{
		return this->generateChecksum(in.data(), (unsigned int)in.size(), type);
	}
	std::string SHA1Generator::generateChecksum(std::string const& in, ChecksumFormats type) const
	{
		return this->generateChecksum(in.data(), (unsigned int)in.size(), type);
	}
}
