#include "connect.hpp"
#include <system_error>
#include <iostream>
#include <byteswap.h>
#include <sstream>
#include <cstring>

using namespace std;

Connection::~Connection() {
    if (!mb) return;
    modbus_close(mb);
    modbus_free(mb);
}

int Connection::Read(int index, int count, uint16_t *buf) {
    int res = modbus_read_registers(mb, index, count, buf);
    //w odczytanej tablicy bajty są w kolejności ba, czyli biblioteka odwraca bajty, my przywracamy stan pierwotny z ramki mobbus
    for(int i=0; i<count; i++) buf[i] = bswap_16(buf[i]);
    return res;
}

/*
-a abcd
-b badc
-c cdab
-d dcba

-e ab
-f ba
*/

/*
{[xsufb][a-f][:index:]}
x - hex
s - signed
u - unsigned
f - float
b - binary
*/

string Connection::Read(int index, const string &format, char order) {
    Result_t result;
    result.u64 = 0;
    int bits;
    int res;

    switch (order) {
        case 'a' ... 'd':
            res = Read(index, 2, result.b16);
            bits = 32;
            break;
        case 'e' ... 'f':
            res = Read(index, 1, result.b16);
            bits = 16;
            break;
        default:
            throw system_error(errno, iostream_category(), "unknown byte order");
    }
    if (res == -1) {
        cerr << "modbus_read_registers index:" << index << endl;
        throw system_error(errno, iostream_category(), modbus_strerror(errno));
    }

    switch (order) {
        case 'a':   //abcd
            result.u32 = bswap_32(result.u32);
            break;
        case 'b':   //badc
        {
            uint16_t temp = result.b16[0];
            result.b16[0] = result.b16[1];
            result.b16[1] = temp;
        }
            break;
        case 'c':   //cdab
            result.b16[0] = bswap_16(result.b16[0]);
            result.b16[1] = bswap_16(result.b16[1]);
            break;
        case 'd':   //dcba
            break;
        case 'e':   //ab
            result.b16[0] = bswap_16(result.b16[0]);
            break;
        case 'f':   //ba
        default:
            break;
    }

    //cout << "format:" << format << " index:" << index << " val:" << result.u64 << endl;

    if (format == "%b") {
        stringstream ss;
        auto mask = (uint32_t)(1ULL << (bits - 1));
        for (int i = 0; i < bits; i++, mask >>= 1) {
            ss << ((result.u32 & mask) ? '1' : '0');
        }
        return ss.str();
    }

    char sbuf[256];

    if(strpbrk(format.c_str(), "fFeEaA")) {
        double a = result.f;
        snprintf(sbuf, sizeof(sbuf) - 1, format.c_str(), a);
    }
    else {
        snprintf(sbuf, sizeof(sbuf) - 1, format.c_str(), result.u64);
    }

    return string(sbuf);
}

Connection::Connection(const Config &cfg) {
    if (cfg.modbusid != -1) {                    //tryb serial
        mb = modbus_new_rtu(cfg.interface.c_str(), cfg.baud, 'N', 8, 1);
        if (mb == NULL) {
            fprintf(stderr, "Unable to create the libmodbus context\n");
            throw system_error(errno, iostream_category(), modbus_strerror(errno));
        }
        if(modbus_set_slave(mb, cfg.modbusid) == -1) {
            cerr << "modbus_set_slave:" << cfg.modbusid << endl;
            throw system_error(errno, iostream_category(), modbus_strerror(errno));
        }
    } else {
        mb = modbus_new_tcp(cfg.interface.c_str(), cfg.port);
        if (mb == NULL) {
            fprintf(stderr, "Unable to allocate libmodbus context\n");
            throw system_error(errno, iostream_category(), modbus_strerror(errno));
        }
    }

    if (modbus_connect(mb) == -1) {
        fprintf(stderr, "Connection %s failed: %s\n", cfg.interface.c_str(), modbus_strerror(errno));
        modbus_free(mb);
        throw system_error(errno, iostream_category(), modbus_strerror(errno));
    }
}
