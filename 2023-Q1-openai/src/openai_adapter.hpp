#ifndef openai_adapter_hpp
#define openai_adapter_hpp


// Standard Library includes
#include <string>


class OpenAIAdapter {
public:
    OpenAIAdapter(const std::string& api_key);
    virtual ~OpenAIAdapter(void);
private:
    const std::string api_key;
    std::string post(const std::string& command, const std::string& data);
};


#endif /* openai_adapter_hpp */
