// Standard Library includes
#include <iostream>

// External Library includes
#include "curl/curl.h"

// Project-level includes
#include "backend.hpp"


int main(int argc, const char * argv[]) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::cout << "Hello, World!\n";
    curl_global_cleanup();
    return 0;
}
