// Standard Library includes
#include <iostream>

// Project-level includes
#include "backend.hpp"


std::string read_from_prompt(const std::string& question) {
    std::string answer;
    std::cout << question;
    std::cin >> answer;
    return answer;
}


int main(int argc, const char * argv[]) {
    const std::string api_key = read_from_prompt("Please enter your API key: ");
    const std::string first_species = read_from_prompt("Please enter species of the first animal: ");
    const std::string second_species = read_from_prompt("Please enter species of the second animal: ");
    std::cout << std::endl;
    Backend backend(api_key, "text-ada-001", 700);
    std::cout << backend.create_joke(first_species, second_species) << std::endl;
    return 0;
}
