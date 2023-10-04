#pragma once

#include <iostream>
#include <ctime>
#include <chrono>

#ifndef _STD_NS_    // namespace std
    #define _STD_NS_
    using namespace std;
#endif              // namespace std

class Logger
{
    public:

    // Member Variables
    FILE *log_file = nullptr;

    // Member Functions
    void initialize(void);
    void print_log(string);
    string get_current_time(void);
    void uninitialize(void);
};
