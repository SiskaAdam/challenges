// Standard Library includes
#include <iostream>

// Project-level includes
#include "backend.hpp"


int main(int argc, const char * argv[]) {
    Backend backend("my api key.");
    std::cout << backend.create_joke("dog", "cat") << std::endl;
    return 0;
}
