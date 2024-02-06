#include "../../include/CLFW/Logger.hpp"

// Class Instance
Logger *Logger::_logger = nullptr;

Logger::Logger(const std::string file)
{
    // Code
    logFile = fopen(file.c_str(), "a+");
    if (logFile == nullptr)
    {
        std::cerr << std::endl << "Failed To Open Log File : logs/Log.txt ... Exiting !!!" << std::endl;
        exit(LOG_ERROR);
    }
}

Logger *Logger::getInstance(const std::string file)
{
    // Code
    if (_logger == nullptr)
        _logger = new Logger(file);

    return _logger;
}

void Logger::printLog(const char* fmt, ...)
{
    // Variable Declarations
    va_list argList;

    // Code
    if (logFile == nullptr)
        return;

    // Print Current Time To File
    fprintf(logFile, "%s", getCurrentTime().c_str());
    fprintf(logFile, "\t");

    // Print Log Data
    va_start(argList, fmt);
    {
        vfprintf(logFile, fmt, argList);
    }
    va_end(argList);

    fprintf(logFile, "\n");
}

std::string Logger::getCurrentTime(void)
{
    // Code
    time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string strTime(30, '\0');
    // strftime(&strTime[0], strTime.size(), "%d/%m/%Y | %I:%M:%S", localtime(&currentTime));
    strftime(&strTime[0], strTime.size(), "%d/%m/%Y | %r", localtime(&currentTime));
    return strTime;
}

void Logger::deleteInstance(void)
{
    delete _logger;
    _logger = nullptr;
}

Logger::~Logger(void)
{
    // Code
    if (logFile)
    {
        #if RELEASE
            printLog("Log File Closed ...");
        #endif
        
        fclose(logFile);
        logFile = nullptr;
    }
}
