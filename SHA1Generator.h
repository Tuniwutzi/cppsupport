#ifndef UTIL_SHA1GENERATOR_H
#define UTIL_SHA1GENERATOR_H

#include <HashGenerator.h>

namespace cppsupport {

class SHA1Generator : public HashGenerator {
public:
    SHA1Generator();

public:
    std::vector<char> generate(std::vector<char> const&) const;
    std::vector<char> generate(char const*, unsigned int) const;
    std::vector<char> generate(std::string const&) const;

    std::string generateChecksum(std::vector<char> const&, ChecksumFormats type = ChecksumFormats::Hex) const;
    std::string generateChecksum(char const*, unsigned int, ChecksumFormats type = ChecksumFormats::Hex) const;
    std::string generateChecksum(std::string const&, ChecksumFormats type = ChecksumFormats::Hex) const;
};

}

#endif
