#include "config.hpp"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

using namespace std;

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

ostream &operator<<(ostream &os, const Config &cfg) {
    os << "Pattern:" << cfg.Pattern << endl;
    os << "ModbusID:" << cfg.modbusid << endl;
    os << "Baudrate:" << cfg.baud << endl;
    os << "Interface:" << cfg.interface << endl;
    os << "TCP Port:" << cfg.port << endl;
    return os;
}

static void Help() {
    cout << "mbprint [opt] \"pattern string\"" << endl;
    cout << "Options:" << endl;
    cout << " -i [serial|ip]        serial device or ip address" << endl;
    cout << " -p [tcp port]         tcp port" << endl;
    cout << " -b [baud]             baud rate" << endl;
    cout << " -d [modbus id]        modbus slave address" << endl;
    cout << " -h                    help" << endl;
    cout << "Pattern string:" << endl;
    cout << " {[:fmt:]:[a-f][:index:]} <=> {[display format]:[byte order][index]]" << endl;
    cout << "Display format:" << endl;
    cout << " like printf function" << endl;
    cout << " is a special switch %b used to display a binary format" << endl;
    cout << "Byte order in modbus frame (A - most significant byte):" << endl;
    cout << " a     - ABCD" << endl;
    cout << " b     - BADC" << endl;
    cout << " c     - CDAB" << endl;
    cout << " d     - DCBA" << endl;
    cout << " e     - AB" << endl;
    cout << " f     - BA" << endl;
    cout << "Index:" << endl;
    cout << " modbus register index count from 0" << endl;
    cout << "Example:" << endl;
    cout << " ./mbprint -i/dev/ttyS0 -b115200 \"Some hex value {%08x:a16} and float {%f:a100}\"" << endl;
}

Config::Config(int argc, char **argv) {
    int opt;

    while ((opt = getopt(argc, argv, "i:p:b:d:h")) != -1) {
        switch (opt) {
            case 'i':
                interface = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'b':
                baud = atoi(optarg);
                break;
            case 'd':
                modbusid = atoi(optarg);
                break;
            case 'h':
                Help();
                exit(0);
            default: /* '?' */
                Help();
                exit(EXIT_FAILURE);
        }
    }


    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

    Pattern = argv[optind];
}
