// Standard Library includes
#include <memory>
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


static size_t receive_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* data_buffer = (std::string*)userp;
    (*data_buffer) += (const char*)contents;
    return size * nmemb;
}


std::string OpenAIAdapter::post(const std::string& command, const std::string& data) {
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
