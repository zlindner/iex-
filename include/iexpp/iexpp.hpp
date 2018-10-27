#ifndef IEXPP_HPP
#define IEXPP_HPP

#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

#define BASE_URL "https://api.iextrading.com/1.0"

using json = nlohmann::json;

namespace iexpp {
    // TODO: make these functions private?

    /*
     * TODO: add info
     *
     * @param endpoint
     * @return
     */
    json get_json(std::string endpoint);

    /*
     * TODO: add info
     *
     * @param in
     * @param size
     * @param num
     * @param out
     * @return
     */
    std::size_t callback(const char *in, std::size_t size, std::size_t num, std::string *out);

    /*
     * TODO: add info
     *
     * @param symbol the symbol to be validated
     * @return the validity of the symbol
     */
    bool valid_symbol(std::string symbol);

    namespace stocks {

        // json batch_request();

        json book(std::string symbol);
    }
}

#endif /* IEXPP_HPP */