#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include <iostream>


class Config {

public:
    Config(int argc, char **argv);

    char *Pattern = NULL;

    std::string interface;
    int port = -1;
    int baud = -1;
    int modbusid = -1;

    friend std::ostream &operator<<(std::ostream &os, const Config &cfg);
};


#endif
