#include "woven.hpp"

int run_application(int argc, char** argv, ostream& os){

    // Return if min number of arguments not met
    if (!validate_args(argc)) {
        return 1;
    }

    string url = argv[1];
    string str_output_number = argv[2]; 
    size_t stoi_size;
    int output_number = 0;

    // Invalidates non-numeric, floats, negatives and mixed values
    try {
        output_number = stoi(str_output_number, &stoi_size);

        if (stoi_size != str_output_number.length()) {
            cerr << "Invalid mix of nums and chars" << endl;
            return 1;
        }

        if (output_number <= 0) {
            cerr << "Negatives/Zero not accepted" << endl;
            return 1;
        }

    } catch (invalid_argument& err) {
        cerr << "Invalid: Not a number" << endl;
        return 1;
    } catch (out_of_range& err) {
        cerr << "Invalid number" << endl;
        return 1;
    }

    // Instance of class URLProcessor
    URLProcessor url_processor(output_number);
    // Instance of class CURLFunctions
    CURLFunctions curl_f;

    // Fetch data
    if (CURLFunctions::curlFromURL(url, url_processor, curl_f) != 0){
        cerr << "Processing error " << endl;
        return 1;
    }

    // Obtain results
    url_processor.getResults(os);

    return 0;
}
