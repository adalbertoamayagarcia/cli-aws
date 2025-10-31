#include "woven.hpp"
#include <sstream> // istringstream
#include <queue>
#include <algorithm> // reverse
#include <cctype>

using namespace std;

/**
 * @brief Function validates the proper number of arguments are passed to the CLI tool.
 *
 * @param argc Count of passed command line arguments
 * @return true if argc >=3, false otherwise
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Check if argc < 3?;
 * if (argc < 3) then (yes)
 *  :"Only (argc) arguments provided, retry...";
 *  :return false;
 * else (no)
 *  :return true;
 * endif
 *
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor run_application
 * participant validate_args
 * participant cout
 *
 * run_application -> validate_args : call validate_args(argc)
 *
 * alt argc < 3
 *  validate_args --> run_application : return false
 *  run_application --> cout : print warning msg
 * else else
 *  validate_args --> run_application : return true
 * end
 *
 * @enduml
 */
bool validate_args(int argc) {
    if (argc < 3){
        cout << "Only " << (argc) << " arguments provided. Retry..." << endl;
        return false;
    }

    return true;
}

// Constructor
URLProcessor::URLProcessor(int num) : output_number(num) {}

/**
 * @brief Function receives CURL buffer of size "size", splits when newline char identified.
 *
 * @param buffer_ptr Pointer to the start of received data
 * @param size Size of received buffer in bytes
 * 
 * @return void
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Initialize buffer = buffer.append(buffer_ptr, size);
 * :Initialize idx = buffer.find("newline char");
 * 
 * while( idx != npos) is (true)
 *  :Initialize line;
 *  :Remove newline char from line;
 *  :Delete parsed content from buffer;
 *  :Call manageHeap(line);
 * endwhile (false)
 *
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor curlCallback
 * participant processReceivedData
 * participant manageHeap
 *
 * curlCallback -> processReceivedData : call processReceivedData(buff_ptr, bytes_to_pass)
 * 
 * alt  idx != npos
 *  processReceivedData -> manageHeap : call manageHeap(line)
 *  manageHeap --> processReceivedData : return
 *  processReceivedData --> curlCallback : return
 * else else
 *  processReceivedData --> curlCallback : return
 * end
 *
 * @enduml
 */
void URLProcessor::processReceivedData(const char* buffer_ptr, size_t size){
    buffer.append(buffer_ptr, size);

    int idx; // index of new line char ('\n')

    /* proceed only if newline is found & remove it from buffer. Else leave data stored in buffer.
     * npos is returned if newline char is NOT found
    */
    while( (idx = buffer.find('\n')) != string::npos) {
        string line = buffer.substr(0, idx); // don't copy newline char
        buffer.erase(0, idx + 1); // skip newline char

        manageHeap(line);
    }
}

/**
 * @brief Function receives a line as argument, splits on white-space
 * and updates heap accordingly.
 *
 * @param line Format <unique_id><white_space><numeric_value>
 * 
 * @return void
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Initialize str_stream(line);
 * :Extract unique_id from str_stream;
 * :Extract num_val from str_stream;
 *
 * :Initialize vector_entry = <num_val, unique_id>;
 *
 * if (min_heap.size() < output_number ) then (yes)
 *  :Push vector_entry to min_heap;
 * elseif (num_val > min_heap.top().first) then (yes)
 *  :Pop min_heap;
 *  :Push vector_entry to min_heap;
 * else
 *  : Do nothing;
 * endif
 * 
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor processReceivedData
 * participant manageHeap
 * entity min_heap
 *
 * processReceivedData -> manageHeap : call manageHeap(const string& line)
 * 
 * alt str_stream != "<unique_id><white space><numeric_value>"
 *  manageHeap --> processReceivedData : return
 * end
 * 
 * alt min_heap_size() < output_number
 *  manageHeap -> min_heap : push(vector_entry)
 *  min_heap --> manageHeap
 * else if num_val > min_heap.top().first
 *  manageHeap -> min_heap : pop()
 *  manageHeap -> min_heap : push(vector_entry)
 *  min_heap --> manageHeap
 * else
 *  note right of manageHeap
 *      Do nothing
 *  end note
 * end
 * 
 * manageHeap --> processReceivedData : return
 *
 * @enduml
 */
void URLProcessor::manageHeap(const string& line) {
    // Create a stream object with passed line
    istringstream str_stream(line);
    string unique_id;
    u_int32_t num_val;

    /* istringstream uses whitespace as a separator, easier than regex.
     * unique_id = line before white-space
     * num_val = line after white-space
     */
    if (!(str_stream >> unique_id >> num_val)) {
        cout << "Invalid line skipped..." << endl;
        return;
    }

    // Push / pop from min_heap if needed, min_heap ensure it pushes in right position, pops from top
    file_line_pair vector_entry(num_val, unique_id);

    if (min_heap.size() < output_number) {
        min_heap.push(vector_entry);
    } else if (num_val > min_heap.top().first) {
        min_heap.pop();
        min_heap.push(vector_entry);
    } else {
        // Do nothing, required per MISRA rules. Not testing MISRA now but good practice
    }
}

/**
 * @brief Function parses/clears min_heap and prints result to stdout.
 *
 * @param os optional ostream to capture stdout for google test, defaults to std::cout
 * 
 * @return void
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Instantiate results vector<file_line_pair>;
 *
 * while (!min_heap.empty() is (true)
 *  :Push value in top of min_heap to the back of results;
 *  :Pop from min_heap;
 * endwhile (false)
 *
 * :Reverse results (descending order);
 * 
 * :Print results to stdout;
 *
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor run_application
 * participant getResults
 * entity results
 * entity reverse
 * entity stdout
 *
 * run_application -> getResults : call getResults()
 *
 * alt min_heap ! empty
 *  getResults -> results : results.push_back(min_heap.top()), min_heap.pop()
 * else else
 *  note right of getResults
 *      Do Nothing
 *  end note
 * end
 *
 * getResults -> reverse : call reverse(results.begin(), results.end()) 
 * getResults -> reverse : call stdout for each element
 *
 * @enduml
 */
void URLProcessor::getResults(ostream& os) {

    vector<file_line_pair> results;

    while (!min_heap.empty()) {
        results.push_back(min_heap.top());
        min_heap.pop();
    }
    reverse(results.begin(), results.end()); // To return in descending order

    for (const auto& [value, u_id] : results) {
        os << u_id << endl;
    }
}

/**
 * @brief Callback function. Triggered on succesful data pull by CURL.
 *
 * @param buff_ptr char pointer to the data
 * @param blk_size size in bytes of the block
 * @param num_blocks number of blocks parsed * 
 * @param userdata Store URLPRrocessor class instance
 * 
 * @return Amount of bytes processed
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Initialize byte_counter = 0;
 * :Initialize byte_counter = 0;
 * :Initialize byte_counter = 0;
 * :Initialize *url_processor = userdata;
 * 
 * :Set bytes_to_pass = blk_size * num_blocks;
 * :Set byte_counter += bytes_to_pass;
 *
 * if (byte_counter <= BYTES_TO_IGNORE) then (yes)
 *  :Return bytes_to_pass;
 * elseif (proceed_flag == false) then (yes)
 *  :Set proceed_flag = true;
 *  :Set bytes_to_pass -= BYTES_TO_IGNORE;
 *  :Set buff_ptr += BYTES_TO_IGNORE + 1;
 *  :Call url_processor->processReceivedData(buff_ptr, bytes_to_pass);
 * :Return bytes_to_pass;
 * else
 * :Call url_processor->processReceivedData(buff_ptr, bytes_to_pass);
 * :Return bytes_to_pass;
 * endif
 * 
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor curlFromURL
 * participant curl_easy_perform
 * participant curlCallback
 * participant processReceivedData
 *
 * curlFromURL -> curl_easy_perform : call curl_easy_perform()
 * curl_easy_perform -> curlCallback : Trigger callback
 * 
 * alt byte_counter <= BYTES_TO_IGNORE
 *  curlCallback --> curlFromURL : return bytes_to_pass
 * else if proceed_flag == false
 * note right of curlCallback
 *  Update bytes_to_pass and buff_ptr
 * end note
 * curlCallback -> processReceivedData : call processReceivedData(buff_ptr,bytes_to_pass)
 * curlCallback --> curlFromURL : return bytes_to_pass
 * else
 * curlCallback -> processReceivedData : call processReceivedData(buff_ptr,bytes_to_pass)
 * curlCallback --> curlFromURL : return bytes_to_pass
 * end
 *
 * @enduml
 */
size_t CURLFunctions::curlCallback(char* buff_ptr, size_t blk_size, size_t num_blocks, void* userdata) {
    static int byte_counter = 0;
    static bool proceed_flag = false; // set to true when past the first 499 bytes
    int bytes_to_pass = 0;
    auto* url_processor = static_cast<URLProcessor*>(userdata);

    if (buff_ptr == nullptr) {
        return -1;
    }

    bytes_to_pass = blk_size * num_blocks;
    byte_counter += bytes_to_pass;
   
    if (byte_counter <= BYTES_TO_IGNORE) {
        return bytes_to_pass;

    } else if (proceed_flag == false) {
        proceed_flag = true;
        bytes_to_pass -= BYTES_TO_IGNORE; // remove 499 bytes
        buff_ptr += BYTES_TO_IGNORE + 1; // move ptr forward to proper position

        url_processor->processReceivedData(buff_ptr, bytes_to_pass);
        return bytes_to_pass + BYTES_TO_IGNORE;

    } else {
        url_processor->processReceivedData(buff_ptr, bytes_to_pass);
        return bytes_to_pass;
    }
}

/**
 * @brief Function uses CURL to fetch data in chunks from a file located in AWS S3 storage.
 *
 * @param url Amazon S3 url for text file
 * @param url_proc URLProcessor class instance
 * @param util CURLFunctions class instance, required for mocking class with google-test
 * 
 * @return False if data processing successful, else True
 *
 * @par Flow Diagram
 * @startuml
 * start
 *
 * :Initialize curl = util.init_curl();
 *
 * if (curl == nullptr) then (yes)
 *  :Return 1;
 * else
 * :Initialize all required curl instance options;
 * :Call curl_easy_perform(curl);
 * 
 * if (curl_res != CURLE_OK) then (yes)
 *  :Print warning;
 * endif
 * 
 * :Call curl_easy_cleanup(curl);
 * 
 * :Return CURLE_OK;
 * 
 * endif
 * 
 * stop
 * @enduml
 *
 * @par Sequence Diagram
 * @startuml
 *
 * actor run_application
 * participant curlFromURL
 * participant utils
 * participant CURL
 *
 * run_application -> curlFromURL : call curlFromURL(url, url_proc, curl_functions)
 * curlFromURL -> utils : call init_curl()
 * utils --> curlFromURL
 * 
 * alt curl == nullptr
 *  curlFromURL --> run_application : return 1
 * end
 * note right of curlFromURL
 *      Initialize curl instance options
 * end note
 * curlFromURL -> CURL : call curl_easy_perform(curl)
 * CURL -> CURL : Trigger callback
 * 
 * alt curl_res != CURLE_OK
 *  note right of curlFromURL
 *      Print warning
 *  end note
 * end
 *
 * curlFromURL --> run_application : Return CURLE_OK
 * @enduml
 */
int CURLFunctions::curlFromURL(const string& url, URLProcessor& url_proc, CURLFunctions& util) {

    CURL* curl = util.init_curl(); // dependency injection

    if (!curl) {
        cout << "curl init failed" << endl;
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // avoid ssl issues with aws s3
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // avoid ssl issues with aws s3
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback); // register callback
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &url_proc);

    CURLcode curl_res = curl_easy_perform(curl);

    /*
     * typedef enum {
     * CURLE_OK = 0,
    */
    if (curl_res != CURLE_OK) {
         std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(curl_res) << "\n";
    }

    curl_easy_cleanup(curl);

    return curl_res == CURLE_OK ? 0 : 1;
}
