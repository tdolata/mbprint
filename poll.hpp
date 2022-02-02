#ifndef _POLL_HPP_
#define _POLL_HPP_

#include "config.hpp"
#include "connect.hpp"
#include <regex>
#include <string>

class Poll {
private:
    static std::regex R;

    static std::string &Replace(std::string &str, const std::string &find, const std::string &rep);

    static std::string GetValue(const std::string &en, Connection &conn);

    const std::string Pattern;
public:
    Poll(Config &cfg) : Pattern(cfg.Pattern) {}

    void Loop(Connection &conn);
};

#endif
