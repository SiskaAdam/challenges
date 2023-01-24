#ifndef backend_hpp
#define backend_hpp


// Standard Library includes
#include <string>


// Project-level includes
#include "openai_adapter.hpp"


class Backend {
public:
    Backend(const std::string& api_key, const std::string& model, const int& max_tokens);
    virtual ~Backend(void);
    std::string create_joke(const std::string& first_species, const std::string& second_species);
private:
    const OpenAIAdapter open_ai;
    const std::string model;
    const int max_tokens;
};


#endif /* backend_hpp */
