#include <iostream>

#include "include/defs.h"
#include "include/parse.h"
#include "include/reader.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << std::string(argv[0]) + " <target file>" << std::endl;
        exit(1);
    }

    std::string t(argv[1]);

    Reader reader(t, t + ".out");
    Parse parse(reader);
    parse.run();
    parse.output_result();

    return 0;
}
