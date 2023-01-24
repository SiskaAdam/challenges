#ifndef openai_adapter_hpp
#define openai_adapter_hpp


// Standard Library includes
#include <optional>
#include <string>


class OpenAIAdapter {
public:
    OpenAIAdapter(const std::string& api_key);
    virtual ~OpenAIAdapter(void);
    
    std::string completion(const std::string& model,
                           const std::optional<std::string>& prompt = std::nullopt,
                           const std::optional<std::string>& suffix = std::nullopt,
                           const std::optional<int>& max_tokens = std::nullopt,
                           const std::optional<double>& temperature = std::nullopt,
                           const std::optional<double>& top_p = std::nullopt,
                           const std::optional<int>& n = std::nullopt,
                           const std::optional<bool>& stream = std::nullopt,
                           const std::optional<int>& logprobs = std::nullopt,
                           const std::optional<bool>& echo = std::nullopt,
                           const std::optional<std::string>& stop = std::nullopt,
                           const std::optional<double>& presence_penalty = std::nullopt,
                           const std::optional<double>& frequency_penalty = std::nullopt,
                           const std::optional<int>& best_of = std::nullopt,
                           const std::optional<std::string>& logit_bias = std::nullopt, // should be a map
                           const std::optional<std::string>& user = std::nullopt) const;
private:
    const std::string api_key;
    std::string post(const std::string& command, const std::string& data) const;
};


#endif /* openai_adapter_hpp */
