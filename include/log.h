//
// Created by shaoliming on 19-9-14.
//
// 为slam系统设计的log模块, 目的就是供其他模块使用简单, 不用关系具体细节, 另外一个就是
// 原因就是为了debug提供便利.

#ifndef SLAM_LOG_H
#define SLAM_LOG_H

#include <string>

#include <mutex>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/core/null_deleter.hpp>

namespace logging = boost::log;

namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace slam
{
enum class severityLevel
{
    debug,
    info,
    warning,
    error,
    critical
};



//! Formatting operator for severity levels
inline std::ostream& operator<< (std::ostream& strm, severityLevel level)
{
    switch (level)
    {
    case severityLevel::debug:
        strm << "debug";
        break;
    case severityLevel::info:
        strm << "info";
        break;
    case severityLevel::warning:
        strm << "warning";
        break;
    case severityLevel ::error:
        strm << "error";
        break;
    case severityLevel::critical:
        strm << "critical";
        break;
    default:
        strm << static_cast< int >(level);
        break;
    }

    return strm;
}


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(globalLogger, src::severity_logger< severityLevel > )


// FIXME: how do we log log the line number and the file name automaticlly?
// logger interface
class logger
{
public:

    virtual void init() = 0;
    virtual void finish() = 0;
//        virtual std::unique_ptr<logger> getDefaultLogger() = 0;
//        virtual std::unique_ptr<logger> getLogger();

    virtual void debug(const std::string &msg) = 0;
    virtual void debugf(const std::string &fmt, ...) = 0;
    virtual void info(const std::string & msg) = 0;
    virtual void infof(const std::string & fmt, ...) = 0;
    virtual void warn(const std::string &msg) = 0;
    virtual void warnf(const std::string &fmt, ...) = 0;
    virtual void error(const std::string &msg) = 0;
    virtual void errorf(const std::string &fmt, ...) = 0;
    virtual void critical(const std::string &msg) = 0;
    virtual void criticalf(const std::string &fmt, ...) = 0;
    virtual ~logger() = default;
};


class myLogger:public logger
{
public:
    using asynchronousSink=sinks::asynchronous_sink<sinks::text_ostream_backend, sinks::bounded_fifo_queue<10000, sinks::drop_on_overflow>>;
    using sinkPtr=boost::shared_ptr<asynchronousSink>;
    constexpr static auto DEFAULT_LOG_FILE_NAME = "slam.log";
private:
    sinkPtr _sink;
    std::string _logFileName;
    severityLevel _level;
    boost::shared_ptr<std::ostream> _fileStrmPtr;
    boost::shared_ptr<std::ostream> _consoleStrmPtr;

public:

    void init() override;
    void finish() override;

    void debug(const std::string &msg) override;
    void debugf(const std::string &fmt, ...) override;

    void info(const std::string &msg) override;
    void infof(const std::string &fmt, ...) override;

    void warn(const std::string &msg) override;
    void warnf(const std::string &fmt, ...) override;

    void error(const std::string &msg) override;
    void errorf(const std::string &fmt, ...) override;

    void critical(const std::string &msg) override;
    void criticalf(const std::string &fmt, ...) override;
private:
    static std::atomic<myLogger*> instance;

    // protect for create myLogger singleton
    static std::mutex initMutex;
    myLogger();
    myLogger(asynchronousSink *sink, std::string logFileName = DEFAULT_LOG_FILE_NAME, severityLevel level = severityLevel::debug);
    // moveable but not copyable
    myLogger(const myLogger &) = delete;
    myLogger& operator=(const myLogger &) = delete;
    void print(const std::string &msg, severityLevel level);
    std::string format(const std::string &fmt, va_list arg);
public:
    static logger*
    getLogger(asynchronousSink *sink = nullptr, std::string logFileName = DEFAULT_LOG_FILE_NAME,
              severityLevel level = severityLevel::debug);

    ~myLogger(){
        _sink->stop();
        _sink->flush();
    }
};
}




#endif //SLAM_LOG_H
