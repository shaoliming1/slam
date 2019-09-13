//
// Created by shaoliming on 19-9-9.
//
// 学习boost log库的
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
namespace logging =  boost::log;
void init()
{
    auto filter = logging::trivial::severity >= logging::trivial::error;
    logging::core::get()->set_filter
            (
                    filter
            );
}

int main(int, char*[])
{
    init();
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return 0;
}