#ifndef _BOARD_DEFINES_
#define _BOARD_DEFINES_

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Create the Ethernet Server port 502 (Modbus TCP)
EthernetServer server(502);

// Create the Modubus Server
ModbusTCPServer modbusTCPServer;
int ModbusDeviceID = 1;				// Modbus Device ID (Node) from 1-255. 

// Define Arduino board PIN Layout. (Here Mega2560)
//Update according to your needs
byte pinsArrayPWM[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 11 PWM Pins - 0-RX, 1-TX, 13-LED are reserved
byte pinsArrayDI[] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 }; // 14 Digital input Pins
byte pinsArrayDO[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 }; // 14 Digital output Pins -- 50-SPI, 51-MOSI, 52-MISO, 53-SCK
byte pinsArrayAI[] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 }; // 16 Analog input pins

#endif