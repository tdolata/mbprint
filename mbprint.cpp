#include "poll.hpp"

using namespace std;

int main(int argc, char **argv) {
    /*
    modbus_t *mb;
    uint16_t tab_reg[32];

    mb = modbus_new_tcp("127.0.0.1", 1502);
    modbus_connect(mb);

    // Read 5 registers from the address 0
    modbus_read_registers(mb, 0, 5, tab_reg);

    modbus_close(mb);
    modbus_free(mb);
    */
    Config config(argc, argv);
    Connection connection(config);
    Poll poll(config);

    poll.Loop(connection);

    return 0;
}
