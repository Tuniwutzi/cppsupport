#pragma once

#include <vector>

namespace cppsupport {

class HashGenerator {
public:
    enum class ChecksumFormats {
        Hex
    };

protected:
    HashGenerator() {}

public:
    virtual ~HashGenerator() {}

public:
    virtual std::vector<char> generate(std::vector<char> const&) const = 0;
    virtual std::vector<char> generate(char const*, unsigned int) const = 0;
    virtual std::vector<char> generate(std::string const&) const = 0;

    virtual std::string generateChecksum(std::vector<char> const&, ChecksumFormats type = ChecksumFormats::Hex) const = 0;
    virtual std::string generateChecksum(char const*, unsigned int, ChecksumFormats type = ChecksumFormats::Hex) const = 0;
    virtual std::string generateChecksum(std::string const&, ChecksumFormats type = ChecksumFormats::Hex) const = 0;

public:
    static HashGenerator& SHA1();
};

}
