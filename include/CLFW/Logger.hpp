#pragma once

#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdarg>

#include "CLFWMacros.hpp"


class Logger
{
    private:
        FILE *logFile = nullptr;
        std::string getCurrentTime(void);

    protected:
        Logger(void);
        static Logger* _logger;

    public:
        //* Non-cloneable
        Logger(Logger &obj) = delete;

        //* Non-assignable
        void operator = (const Logger &) = delete;

        Logger(const std::string file);
        ~Logger(void);

        // Member Function Declarations
        static Logger* getInstance(const std::string file);
        void printLog(const char* fmt, ...);
        void deleteInstance(void);
};

