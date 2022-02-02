#ifndef _CONNECT_HPP_
#define _CONNECT_HPP_

#include <cstdint>
#include <modbus.h>
#include <string>
#include "config.hpp"
#include <string>

typedef union {
    uint16_t b16[4];
    uint16_t u16;
    int16_t s16;
    uint32_t u32;
    int32_t s32;
    uint64_t u64;
    int64_t s64;
    float f;
    double d;
    uint8_t b8[8];
} Result_t;

class Connection {
private:
    modbus_t *mb = NULL;
public:
    Connection(const Config &cfg);

    virtual ~Connection();

    int Read(int index, int count, uint16_t *buf);

    std::string Read(int index, const std::string &format, char order);
};

#endif
