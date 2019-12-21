# WHAT IS THIS?

This programm allows to use your Arduino as a 
Fieldbus Coupler over Modbus TCP. This means you can 
Read/Write Digital and Analog In/Outputs from a
higher level controller. For example a PLC or PC.

## HOW TO USE?

*Update the file **"boardDefines.h"** according to your Arduino board and your needs.
1. adapt your IP, MAC, ... 
2. Update the arrays "pinsArrayDI" & "pinsArrayDO" with the pin_numbers of your board.
You can use all of them as DI or DO. Or any combination such as:

![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/boardDefines.jpg?raw=true)


*Then Build and Load into the arduino board. Serial Monitor shows something like this:

![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/Initialize_Serial.jpg?raw=true)

*Use any ModbusTCP-Client to connect from your PC to the Arduino. Serial monitor should say "new client"

![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/Modscan_Connect.jpg?raw=true)

*Watch inputs and set outputs from the corresponding registers (1x, 2x, 3x, 4x). For example:

*Set Coils  (0x) to Arduino Digital Outputs
![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/DO.jpg?raw=true)

*Read Arduino Digital inputs into Discrete Inputs registers (1x)
![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/DI.jpg?raw=true)

*Modbus Input Registers (3x)
![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/AI.jpg?raw=true)

*Modbus Holding Registers (4x)
![Preview](https://github.com/rocadura/Arduino-Modbus_TCP_Server/blob/master/pics/AO.jpg?raw=true)


## SCALING ANALOG VALUES

Analog values are scaled from 0 to 100,0% using an integer with 10x Gain.
I.e. - 1000 Modbus = 100% in Arduino\
	 - 855 Modbus = 85,5% in Arduino\
	 - 151 Modbus = 15,1% in Arduino\

## HARDWARE REQUIREMENTS:
	Arduino MEGA 2560\
	Ethernet Shield W5100 for Arduino

## SOFTWARE REQUIREMENTS (TESTED WITH):
	Microsoft Visual Studio 2013 using Arduino IDE v1.6/1.8

	###Arduino Libraries:\
		- ArduinoRS485 1.0.0\
		- ArduinoModbus by Arduino 1.0.0\
		- SPI Master library for arduino (built-in)\
		- Ethernet library for arduino (built-in)
