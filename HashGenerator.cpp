#include <HashGenerator.h>

#include <SHA1Generator.h>

namespace cppsupport {

HashGenerator& HashGenerator::SHA1() {
    static SHA1Generator gen;
    return gen;
}

}
