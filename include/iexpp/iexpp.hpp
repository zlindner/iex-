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

    /*
     * Helper function for getting the string representation of a boolean value
     * @param b the boolean to be converted to a string
     * @return the string representation of the passed boolean value
     */
    std::string bool_to_string(bool b);

    namespace stocks {

        typedef enum class range { FIVE_Y, TWO_Y, ONE_Y, YTD, SIX_M, THREE_M, ONE_M, ONE_D, DATE, DYNAMIC } range;

        inline std::string range_to_string(range r);

        // json batch_request();

        json book(std::string symbol);

        json chart(std::string symbol, range r, bool chart_reset = false, bool chart_simplify = false, int chart_interval = 1, bool change_from_close = false, int chart_last = 0);
    }
}

#endif /* IEXPP_HPP */