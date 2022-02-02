# mbprint
An application that displays Modbus registers in a similar way to printf.

mbprint [opt] "pattern string"<br>
Options:<br>
 -i [serial|ip]        serial device or ip address<br>
 -p [tcp port]         tcp port<br>
 -b [baud]             baud rate<br>
 -d [modbus id]        modbus slave address<br>
 -h                    help<br>

Pattern string:<br>
 {[:fmt:]:[a-f][:index:]} <=> {[display format]:[byte order][index]]<br>

Display format:<br>
 like printf function<br>
 is a special switch %b used to display a binary format<br>
Byte order in modbus frame (A - most significant byte):<br>
 a     - ABCD<br>
 b     - BADC<br>
 c     - CDAB<br>
 d     - DCBA<br>
 e     - AB<br>
 f     - BA<br>
Index:<br>
 modbus register index count from 0<br>

Example:<br>
**mbprint -d1 -i/dev/ttyS0 -b115200 "Some hex value {%08x:a16} and float {%f:a100}"**<br>
