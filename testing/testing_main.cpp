#include "woven.hpp"
#include "runner.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

// Test runner file with dummy url, valid number of args
TEST(ValidateRunner, Runner_DummyUrl_ValidArgs) {
    const char* argv [] = {"./dummy.sh", "https://dummy_url.com/dummy_file.txt", "2"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test runner file with dummy url, invalid number of args
TEST(ValidateRunner, Runner_DummyUrl_InvalidArgs) {
    const char* argv [] = {"./dummy.sh", "dummy_url"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test runner file with proper url and proper arg size
TEST(ValidateRunner, Runner_ValidUrl_ValirArgs) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "2"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 0);
}

// Test runner file with proper url and invalid arg size
TEST(ValidateRunner, Runner_ValidUrl_InvalidArgSize) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending special character for N
TEST(ValidateRunner, Runner_ValidUrl_SpecialChar) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "@"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending mix of integers and chars
TEST(ValidateRunner, Runner_ValidUrl_CharAndInt) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "123abc"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending a negative integer
TEST(ValidateRunner, Runner_ValidUrl_NegativeInt) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "-1"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending a float
TEST(ValidateRunner, Runner_ValidUrl_NegativeFloat) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "3.4"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending out of range int
TEST(ValidateRunner, Runner_ValidUrl_OutOfRange) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt ", "100000000000"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    int result = run_application(argc, const_cast<char**>(argv));

    EXPECT_EQ(result, 1);
}

// Test sending fewer arguments than expected
TEST(ValidateArgs, TooFewArgs) {
    EXPECT_FALSE(validate_args(0));
    EXPECT_FALSE(validate_args(1));
    EXPECT_FALSE(validate_args(2));
}

// Test correct number of arguments
// Test more than expected, should just be ignored
TEST(ValidateArgs, CorrectArgs) {
    EXPECT_TRUE(validate_args(3));
    EXPECT_TRUE(validate_args(4));
    EXPECT_TRUE(validate_args(4));
}


// Test TestCurlCallback with nullptr
TEST(TestCurlCallback, nullPointer) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    void* userdata;

    size_t x = 0;
    size_t y = 0;
    size_t result = 0;

    result = CURLFunctions::curlCallback(nullptr, x, y, userdata);

    EXPECT_EQ(result, -1);
}

// Test curlCallback passing <= BITS_TO_IGNORE (499)
TEST(TestCurlCallback, bitsToIgnore) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    void* userdata;
    char* buffptr;

    size_t x = 1;
    size_t y = 499;
    size_t result = 0;

    result = CURLFunctions::curlCallback(buffptr, x, y, userdata);

    EXPECT_EQ(result, 499);
}

// Test heap size equals 1
TEST(ValidateHeap, TestHeapSize_1) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    URLProcessor proc(stoi(argv[2]));
    string line = "fd39a3d8e96825f3777 336";

    proc.manageHeap(line);

    EXPECT_EQ(proc.getMinHeap().size(), 1);
}

// Test heap size equals 2
TEST(ValidateHeap, TestHeapSize_2) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    URLProcessor proc(stoi(argv[2]));
    vector<string> lines = {"fd39a3d8e96825f3777 336",
                            "772b431822d5664ea5f 437"
    };

    for(string& line : lines){
        proc.manageHeap(line);
    }

    EXPECT_EQ(proc.getMinHeap().size(), 2);
}

// Test heap size equals 3
TEST(ValidateHeap, TestHeapSize_3) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    URLProcessor proc(stoi(argv[2]));

    vector<string> lines = {"fd39a3d8e96825f3777 336",
                            "772b431822d5664ea5f 437",
                            "78eab4ccbdd98fa911e 1886"
    };

    for (string& line: lines) {
        proc.manageHeap(line);
    }

    EXPECT_EQ(proc.getMinHeap().size(), 3);

}

// Test heap size still equals 3
TEST(ValidateHeap, TestHeapSize_still3) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    URLProcessor proc(stoi(argv[2]));
    vector<string> lines = {"fd39a3d8e96825f3777 336",
                            "772b431822d5664ea5f 437",
                            "78eab4ccbdd98fa911e 1886",
                            "a8f962e650be6e090d0 1595"
    };

    for (string& line : lines) {
        proc.manageHeap(line);
    }

    EXPECT_EQ(proc.getMinHeap().size(), 3);
}

// Test outcome - top 3 heap values match
TEST(ValidateHeap, TestHeapResult_3) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    int idx = 0;

    stringstream buffer;
    string line;

    string expected_uids [] = {"21d8d9157863db8d566",
                              "e4ca9829fe4c961c9b7",
                              "d418cc1a140a1e27672"
    };

    URLProcessor proc(stoi(argv[2]));

    int result = run_application(argc, const_cast<char**>(argv), buffer);

    while(getline(buffer, line)) {
        cout << "line:" << line << endl;
        cout << "expected:" << expected_uids[idx] << endl;
        EXPECT_EQ(line, expected_uids[idx]);
        idx++;
    }

    EXPECT_EQ(result, 0);
}

// Test outcome - top 5 heap values match
TEST(ValidateHeap, TestHeapResult_5) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "5"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    int idx = 0;

    stringstream buffer;
    string line;

    string expected_uids [] = {"21d8d9157863db8d566",
                               "e4ca9829fe4c961c9b7",
                               "d418cc1a140a1e27672",
                               "95b970f2883b8e0110d",
                               "863b5d210f91ac20bf9"
    };

    URLProcessor proc(stoi(argv[2]));

    int result = run_application(argc, const_cast<char**>(argv), buffer);

    while(getline(buffer, line)) {
        cout << "line:" << line << endl;
        cout << "expected:" << expected_uids[idx] << endl;
        EXPECT_EQ(line, expected_uids[idx]);
        idx++;
    }

    EXPECT_EQ(result, 0);
}

// Test outcome - top 10 heap values match
TEST(ValidateHeap, TestHeapResult_10) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "10"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    int idx = 0;

    stringstream buffer;
    string line;

    string expected_uids [] = {"21d8d9157863db8d566",
                               "e4ca9829fe4c961c9b7",
                               "d418cc1a140a1e27672",
                               "95b970f2883b8e0110d",
                               "863b5d210f91ac20bf9",
                               "8a694511c44d841d7e9",
                               "60d84237d76fc2a434f",
                               "414bce17d8b5e3f8741",
                               "f288ac9e8c07fac90c6",
                               "100ae6cdd3eb8f64e55"
    };

    URLProcessor proc(stoi(argv[2]));

    int result = run_application(argc, const_cast<char**>(argv), buffer);

    while(getline(buffer, line)) {
        cout << "line:" << line << endl;
        cout << "expected:" << expected_uids[idx] << endl;
        EXPECT_EQ(line, expected_uids[idx]);
        idx++;
    }

    EXPECT_EQ(result, 0);
}

// Test istringstream (manageHeap) with an invalid line format
TEST(ValidateHeap, TestHeap_invalidLine) {
    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    URLProcessor proc(stoi(argv[2]));
    string line = "fd39a3d8e96825f3777336";

    proc.manageHeap(line);

    EXPECT_EQ(proc.getMinHeap().size(), 0);
}

class MockCURLFunctions : public CURLFunctions {
public:
    MOCK_METHOD(CURL*, init_curl, (), (override));
};

// Test curlFromURL, create a Mock to generate a nullptr
TEST(TestCurlFromURL, nullPointer) {

    const char* argv [] = {"./dummy.sh", "https://woven-adalberto.s3.us-east-2.amazonaws.com/sw-engineer-challenge.txt", "3"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    string url = argv[1];

    MockCURLFunctions mock;
    URLProcessor proc(stoi(argv[2]));

    EXPECT_CALL(mock, init_curl())
        .WillOnce(Return(nullptr));

    int result = CURLFunctions::curlFromURL(url, proc, mock);

    EXPECT_EQ(result, 1);
}

// ############################################################################33
int main(int argc, char** argv) {

    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
