#include "iexpp/iexpp.hpp"

// TODO add usings

int main() {
    //

    std::cout << iexpp::stocks::chart("AAPL", iexpp::stocks::range::FIVE_Y) << std::endl;

    return 0;
}

/*
 * IEXPP
 */

std::size_t iexpp::callback(const char *in, std::size_t size, std::size_t num, std::string *out) {
    const std::size_t total_bytes(size * num);

    out->append(in, total_bytes);

    return total_bytes;
}

json iexpp::get_json(std::string endpoint) {
    json obj;
    std::string url(BASE_URL);
    CURL *curl;
    CURLcode code;
    std::unique_ptr<std::string> data(new std::string());

    url += endpoint;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        std::cout << "Error initializing curl" << std::endl;

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // don't use IPv6
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);                  // 10 second timeout
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data.get());

    code = curl_easy_perform(curl);

    if (code == CURLE_OK) {
        std::cout << "Successful response from " << url << std::endl;

        if (*data == "Not Found") {
            std::cout << "Invalid api call" << std::endl;
            obj = NULL;
        } else {
            obj = json::parse(*data);
        }
    } else {
        std::cout << "Couldn't GET from " << url << std::endl;
        std::cout << curl_easy_strerror(code) << std::endl;
        obj = NULL;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return obj;
}

bool iexpp::valid_symbol(std::string symbol) {
    json symbols;

    // TODO: move to reference_data namespace (iexpp::reference_data::symbols())
    symbols = iexpp::get_json("/ref-data/symbols");

    if (symbols != NULL) {
        for (json::iterator iter = symbols.begin(); iter != symbols.end(); ++iter) {
            if ((*iter).at("symbol") == symbol) {
                return true;
            }
        }
    }

    std::cout << "Symbol: " + symbol + " doesn't exist" << std::endl;

    return false;
}

inline std::string iexpp::bool_to_string(bool b) { return b ? "true" : "false"; }

/*
 * STOCKS
 */

std::string iexpp::stocks::range_to_string(iexpp::stocks::range r) {
    switch (r) {
    case iexpp::stocks::range::FIVE_Y:
        return "5Y";
    case iexpp::stocks::range::TWO_Y:
        return "2Y";
    case iexpp::stocks::range::ONE_Y:
        return "1Y";
    case iexpp::stocks::range::YTD:
        return "YTD";
    case iexpp::stocks::range::SIX_M:
        return "6M";
    case iexpp::stocks::range::THREE_M:
        return "3M";
    case iexpp::stocks::range::ONE_M:
        return "1M";
    case iexpp::stocks::range::ONE_D:
        return "1D";
    case iexpp::stocks::range::DATE:
        return "DATE";
    case iexpp::stocks::range::DYNAMIC:
        return "DYNAMIC";
    default:
        return "";
    }
}

// batch

json iexpp::stocks::book(std::string symbol) {
    json book;

    if (!valid_symbol(symbol)) {
        return NULL;
    }

    book = get_json("/stock/" + symbol + "/book");

    return book;
}

json iexpp::stocks::chart(std::string symbol, iexpp::stocks::range r, bool chart_reset, bool chart_simplify, int chart_interval, bool change_from_close, int chart_last) {
    json chart;
    std::string options;
    std::string params;

    if (!valid_symbol(symbol)) {
        return NULL;
    }

    options = "/" + iexpp::stocks::range_to_string(r);

    params = "?chartReset=" + bool_to_string(chart_reset);
    params += "?chartSimplify=" + bool_to_string(chart_simplify);
    params += "?chartInterval=" + std::to_string(chart_interval);
    params += "?changeFromClose=" + bool_to_string(change_from_close);
    params += "?chartLast=" + std::to_string(chart_last);

    chart = get_json("/stock/" + symbol + "/chart" + options + params);

    return chart;
}