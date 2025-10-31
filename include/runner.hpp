#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
using namespace std;

// wrapper on top if main for easier unit_test execution (avoid having 2 mains)
// optional ostream added to capture stdout in google test, defautlds to std::cout
int run_application(int argc, char** argv, ostream& os = cout);

#endif
