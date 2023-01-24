// Standard Library includes
#include <string>

// Project-level includes
#include "backend.hpp"


Backend::Backend(const std::string& api_key, const std::string& model, const int& max_tokens): open_ai(api_key), model(model), max_tokens(max_tokens) { /* Nothing to create */ }

Backend::~Backend(void) { /* Nothing to destroy */ }

std::string Backend::create_joke(const std::string& first_species, const std::string& second_species) {
    static const std::string part_01 = std::string("Tell me a joke. The joke must involve two or more animals. The species of at least one involved animal must be '");
    static const std::string part_02 = std::string("'. The species of at least (another) one involved animal must be '");
    static const std::string part_03 = std::string("'.");
    const std::string request = part_01 + first_species + part_02 + second_species + part_03;
    // TODO: this is just a first attempt, needing some more polishment.
    return open_ai.completion(model, request, std::nullopt, max_tokens);
}
