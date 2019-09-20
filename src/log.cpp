//
// Created by shaoliming on 19-9-14.
//
#include <iosfwd>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/record_ordering.hpp>
#include "log.h"

namespace slam
{
// const  std::string slam::myLogger::DEFAULT_LOG_FILE_NAME = "slam_%N.log";
std::atomic<myLogger *> slam ::myLogger::instance;
std::mutex slam::myLogger::initMutex;

slam::myLogger::myLogger()
{

}

slam::myLogger::myLogger(slam::myLogger::asynchronousSink *sink, std::string logFileName, severityLevel level):_sink(sink), _logFileName(logFileName), _level(level) {
    _consoleStrmPtr = boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter());
    _fileStrmPtr = boost :: shared_ptr<std::ostream> (new std::ofstream(_logFileName));
}

slam::logger* slam::myLogger::getLogger(slam::myLogger::asynchronousSink *sink, std::string logFileName, severityLevel level)
{
    auto ptr = instance.load();
    if(!ptr)
    {
        std::lock_guard<std::mutex> lock(initMutex);
        if(!sink)
        {
            sink = new asynchronousSink;
            if(sink == nullptr)
            {
                std::cerr << "exhaused memory!" << std::endl;
                throw std::runtime_error("allocate memory for asynchronousSink failed");
            }
        }
        ptr = instance.load(std::memory_order_relaxed);
        if(!ptr)
        {
            ptr = new myLogger(sink, logFileName, level);
            if(ptr == nullptr)
            {
                std::cerr << "exhaused memory!" << std::endl;
                throw std::runtime_error("allocate memory for myLogger failed");
            }
            instance.store(ptr, std::memory_order_release);
            ptr->init();
        }
    }
    return ptr;
}

void myLogger::init()
{
    try
    {


        if (!_fileStrmPtr->good())
            throw std::runtime_error("Failed to open a text log file");

        // Add output stream
        _sink->locked_backend()->add_stream(_fileStrmPtr);
        _sink->locked_backend()->add_stream(_consoleStrmPtr);

        // set the global filter
        _sink->set_filter(expr::attr<severityLevel>("Severity") >= _level);

        _sink->set_formatter
        (
            expr::format("RecordID %1%: [%2%] Thread ID : [%3%] <%4%> - %5%")
            % expr::attr< unsigned int >("RecordID")
            % expr::attr< boost::posix_time::ptime >("TimeStamp")
            % expr::attr< attrs::current_thread_id::value_type >("ThreadID")
            % expr::attr< severityLevel>("Severity")
            % expr::message
        );

        // Add it to the core
        logging::core::get()->add_sink(_sink);

        // Add some attributes too
        logging::add_common_attributes();
        logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
        logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
        logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
    }
    catch (std::exception& e)
    {
        std::cout << "FAILURE: " << e.what() << std::endl;
        exit(-1);
    }
}

void myLogger::finish(){
    auto p = instance.load();
    if(p){
        delete p;
    }
}

void myLogger::print(const std::string &msg, severityLevel level)
{
    auto slg = globalLogger::get();
    BOOST_LOG_SEV(slg,level) << msg;
}

std::string myLogger::format(const std::string &fmt, va_list arg)
{
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    //va_list ap;
    va_list  ap;
    while (1)       // Maximum two passes on a POSIX system...
    {
        str.resize(size);
        va_copy(ap, arg);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)    // Everything worked
        {
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

void myLogger::debug(const std::string &msg)
{
    print(msg, severityLevel::debug);
}

void myLogger::debugf(const std::string &fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    auto ret = format(fmt, args);
    va_end(args);
    return debug(ret);
}

void myLogger::info(const std::string &msg)
{
    print(msg, severityLevel::info);
}

void myLogger::infof(const std::string &fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    auto ret = format(fmt, args);
    va_end(args);
    return info(ret);
}

void myLogger::warn(const std::string &msg)
{
    print(msg, severityLevel::warning);
}

void myLogger::warnf(const std::string &fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    auto ret = format(fmt, args);
    va_end(args);
    return warn(ret);
}

void myLogger::error(const std::string &msg)
{
    print(msg, severityLevel::error);
}

void myLogger::errorf(const std::string &fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    auto ret = format(fmt, args);
    va_end(args);
    return error(ret);
}

void myLogger::critical(const std::string &msg)
{
    print(msg, severityLevel::critical);
}

void myLogger::criticalf(const std::string &fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    auto ret = format(fmt, args);
    va_end(args);
    return critical(ret);
}

}
