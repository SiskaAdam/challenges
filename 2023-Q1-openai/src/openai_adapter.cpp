// Standard Library includes
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

// External Library includes
#include "curl/curl.h"

// Project-level includes
#include "openai_adapter.hpp"


static const std::string OPEN_AI_URL = std::string("https://api.openai.com/");
static const std::string OPEN_AI_CONTENT_TYPE = std::string("Content-Type: application/json");
static const std::string OPEN_AI_AUTH_PREFIX = std::string("Authorization: Bearer ");
static const std::string OPEN_AI_COMMAND_PREFIX = OPEN_AI_URL + "v1/";
static const std::string OPEN_AI_COMMAND_COMPLETION = std::string("completions");


class CURLGlobalLoader final {
public:
    static void init(void) {
        if (!singleton_instance) {
            singleton_instance = std::unique_ptr<CURLGlobalLoader>(new CURLGlobalLoader());
        }
    }
    
    CURLGlobalLoader(const CURLGlobalLoader&) = delete;
    
    ~CURLGlobalLoader(void) {
        curl_global_cleanup();
    }
    
private:
    static std::unique_ptr<CURLGlobalLoader> singleton_instance;
    
    CURLGlobalLoader(void) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
};


class CURLSession final {
public:
    CURLSession(void): session(curl_easy_init()) { }
    CURLSession(const CURLSession&) = delete;
    
    ~CURLSession(void) {
        if (session) {
            curl_easy_cleanup(session);
            session = nullptr;
        }
    }
    
    CURL* get(void) {
        return session;
    }
    
private:
    CURL* session;
};


std::unique_ptr<CURLGlobalLoader> CURLGlobalLoader::singleton_instance = nullptr;


OpenAIAdapter::OpenAIAdapter(const std::string& api_key): api_key(api_key) {
    CURLGlobalLoader::init();
}

OpenAIAdapter::~OpenAIAdapter(void) { /* Nothing to destroy */ }


std::string OpenAIAdapter::completion(const std::string& model,
                                      const std::optional<std::string>& prompt,
                                      const std::optional<std::string>& suffix,
                                      const std::optional<int>& max_tokens,
                                      const std::optional<double>& temperature,
                                      const std::optional<double>& top_p,
                                      const std::optional<int>& n,
                                      const std::optional<bool>& stream,
                                      const std::optional<int>& logprobs,
                                      const std::optional<bool>& echo,
                                      const std::optional<std::string>& stop,
                                      const std::optional<double>& presence_penalty,
                                      const std::optional<double>& frequency_penalty,
                                      const std::optional<int>& best_of,
                                      const std::optional<std::string>& logit_bias,
                                      const std::optional<std::string>& user) const {
    // TODO: this could surely be implemented better. A JSON library should do the job.
    std::string data = std::string("{\"model\": \"") + model + "\"";
    if (prompt) {
        data += ", \"prompt\": \"" + prompt.value() + "\"";
    }
    if (max_tokens) {
        data += ", \"max_tokens\": " + std::to_string(max_tokens.value());
    }
    if (temperature) {
        data += ", \"temperature\": " + std::to_string(temperature.value());
    }
    if (top_p) {
        data += ", \"top_p\": " + std::to_string(top_p.value());
    }
    // TODO: am not going to add all options...
    return post(OPEN_AI_COMMAND_COMPLETION, data + "}");
}


static size_t receive_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* data_buffer = (std::string*)userp;
    (*data_buffer) += (const char*)contents;
    return size * nmemb;
}


std::string OpenAIAdapter::post(const std::string& command, const std::string& data) const {
    CURLSession curl_session;
    CURL* curl = curl_session.get();
    if (!curl) {
        throw std::runtime_error("Unable to initialise Curl.");
    }
    curl_easy_setopt(curl, CURLOPT_URL, (OPEN_AI_COMMAND_PREFIX + command).c_str());

    struct curl_slist *header = NULL;
    header = curl_slist_append(header, OPEN_AI_CONTENT_TYPE.c_str());
    header = curl_slist_append(header, (OPEN_AI_AUTH_PREFIX + api_key).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());

    std::string received_data;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&received_data);

    const CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        throw std::runtime_error("Curl call to server failed.");
    }
    
    return received_data;
}
