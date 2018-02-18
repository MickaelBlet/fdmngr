#include <iostream>

#include "blet/conf.h"
#include "test/already_include.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    blet::Dict conf;
    conf["main"] = "foobar";
    test(conf);
    std::cout << blet::conf::dump(conf, 2) << std::endl;
    return 0;
}
