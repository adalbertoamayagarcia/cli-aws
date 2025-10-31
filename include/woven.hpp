#ifndef WOVEN_HPP
#define WOVEN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <curl/curl.h>

using namespace std;

#define BYTES_TO_IGNORE 499

/* ################################## ALIASES ######################################*/
using file_line_pair = pair<u_int32_t, string>; // One for each line in remote file (numeric_value, unique_id), num_value first for min_heap management

/* ###################################### CLASS ######################################*/
class URLProcessor {
public:

    URLProcessor(int num);

    // process received data chunk from CURL
    void processReceivedData(const char* buffer_ptr, size_t size);

    // heap management, ensure N size, push and pop as required
    void manageHeap(const string& line);

    // print results to stdout, added ostream optional argument to capture output for google test (defaults to std::cout)
    void getResults(ostream& os = cout);

private:

    string buffer;
    int output_number;

    priority_queue<file_line_pair, vector<file_line_pair>, greater<>> min_heap; // <data type, data container, operator>
    /* CODE COMPLEXITY for a MIN_HEAP
     * n = number of requested output
     *
     * pushing a value  -> push(x)  -> O(log n)
     * looking at top   -> top()    -> O(1)
     * Popping a value  -> pop()    -> O(log n)
     * 
     * MEMORY -> O(n)
    */

public:
#ifdef UNIT_TEST
    auto& getMinHeap() {
        return min_heap;
    }
#endif
};

class CURLFunctions {
public:

    // virtual, hence can be overriden and mocked by google test
    virtual CURL* init_curl() {
        return curl_easy_init();
    }

    // CURL fetch data, added dependency injection to mock it later
    static int curlFromURL(const string& url, URLProcessor& url_proc, CURLFunctions& util);

    // callback for CURL
    static size_t curlCallback(char* buff_ptr, size_t blk_size, size_t num_blocks, void* userdata);

    // destructor
    virtual ~CURLFunctions() = default;
};

/* ################################## HELPER FUNCTIONS ######################################*/
// validate minimum amount of arguments are provided
bool validate_args(int argc);

#endif
