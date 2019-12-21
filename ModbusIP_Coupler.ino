/*******************************************************************************************
* Arduino - Modbus TCP Server - Reads and Writes Hardware Inputs/Outputs into Modbus Regs  *
* Based on:    See readme.md                                                               *
* Author:      Juan Reyes, Dec, 2019.                                                      *
* Copyright:   Unlicense -> For more information, please refer to <http://unlicense.org>   *
* Use:		   See readme.md                                                               *
********************************************************************************************/

// Include standard libraries
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <SPI.h>
#include <Ethernet.h>
#include"boardDefines.h" // Define User Types below here or use a .h file

// The setup() function runs once each time the micro-controller starts
void setup()
{
	initialize();	// Start Serial, Ethernet and Modbus communication
	pinSetup();		// Setup the DI/DO according to "boardDefines.h"
}

// Add the main program code into the continuous loop() function
void loop()
{
	runModbusServer();
}

/********************************************************************************************/

void initialize()
{
	//Initialize serial and wait for port to open:
	Serial.begin(9600);
	while (!Serial) { ; }											// wait for serial port to connect. Needed for native USB port only
	Serial.println("Initialize Serial communication succeed");

	Ethernet.begin(mac, ip, myDns, gateway, subnet);				// Initialize Ethernet module

	if (Ethernet.hardwareStatus() == EthernetNoHardware)			// Check for Ethernet hardware present
	{
		Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
		while (true) {delay(1);}									// Do nothing, no point running without Ethernet hardware
	}

	if (Ethernet.linkStatus() == LinkOFF)							// Check for Ethernet cable connection
	{
		Serial.println("Ethernet cable is not connected.");
	}

	server.begin();													// Start Ethernet server listening for clients and print IP Address
	
	// Print Debugs
	Serial.print("Modbus server address:");
	Serial.println(Ethernet.localIP());
	Serial.println("Device MAC address: ");
	Serial.print(mac[0],HEX);
	Serial.print("-");
	Serial.print(mac[1],HEX);
	Serial.print("-");
	Serial.print(mac[2],HEX);
	Serial.print("-");
	Serial.print(mac[3],HEX);
	Serial.print("-");
	Serial.print(mac[4],HEX);
	Serial.print("-");
	Serial.println(mac[5],HEX);
	
	if (!modbusTCPServer.begin(ModbusDeviceID))						// start the Modbus TCP server
	{
		Serial.println("Failed to start Modbus TCP Server!");
		while (1);
	}
	
	// Print Debugs
	Serial.print("Modbus device ID: ");
	Serial.println(ModbusDeviceID);

	// configure Modbus addresses 0x00
	modbusTCPServer.configureCoils(0x0000, (sizeof(pinsArrayDO)));
	modbusTCPServer.configureDiscreteInputs(0x0000, sizeof(pinsArrayDI));
	modbusTCPServer.configureInputRegisters(0x0000, sizeof(pinsArrayAI));
	modbusTCPServer.configureHoldingRegisters(0x0000, sizeof(pinsArrayPWM));

	// Print Debugs
	Serial.println("Initialize Modbus TCP Server succeed");
}

void pinSetup()
{
	Serial.println("Pin Setup started");

	// Pin setup digital inputs
	for (int i = 0; i < sizeof(pinsArrayDI); i = i + 1)
	{
		pinMode(pinsArrayDI[i], INPUT);
	}

	// Pin setup digital outputs
	for (int i = 0; i < sizeof(pinsArrayDO); i = i + 1)
	{
		pinMode(pinsArrayDO[i], OUTPUT);
	}

	// Pin setup analog outputs PWM
	for (int i = 0; i < sizeof(pinsArrayPWM); i = i + 1)
	{
		pinMode(pinsArrayPWM[i], OUTPUT);
	}

	pinMode(LED_BUILTIN, OUTPUT);

	Serial.print(sizeof(pinsArrayDI));
	Serial.print("-DIs, ");
	Serial.print(sizeof(pinsArrayDO));
	Serial.print("-DOs, ");
	Serial.print(sizeof(pinsArrayAI));
	Serial.print("-AIs and ");
	Serial.print(sizeof(pinsArrayPWM));
	Serial.println("-AOs (PWM)");
	Serial.println("Pin Setup succeed ");

}

void runModbusServer()
{
	EthernetClient client = server.available();		// wait for a new client:

	if (client)										// when the client sends the first byte, say hello:
	{
		Serial.println("new client");				// a new client connected
		modbusTCPServer.accept(client);				// let the Modbus TCP accept the connection
		while (client.connected())
		{
			digitalWrite(LED_BUILTIN, HIGH);		// Led ON if client is connected
			updateIO();								// Read/write Hardware-inputs/outputs
			modbusTCPServer.poll();					// poll for Modbus TCP requests, while client connected
		}
		digitalWrite(LED_BUILTIN, LOW);
		Serial.println("client disconnected");
	}

}

void updateIO()
{
	// Read Modbus Coils (0x) Register and set the hardware output accordingly.
	for (int i = 0; i < sizeof(pinsArrayDO); i = i + 1)
	{
		digitalWrite(pinsArrayDO[i], modbusTCPServer.coilRead(i));
	}

	// Read Hardware inputs and store them into the Modbus Discrete Inputs (1x) Registers 
	for (int i = 0; i < sizeof(pinsArrayDI); i = i + 1)
	{
		modbusTCPServer.discreteInputWrite(i, digitalRead(pinsArrayDI[i]));
	}

	// Read Hardware inputs and store them into the Modbus Input Registers (3x)
	for (int i = 0; i < sizeof(pinsArrayAI); i = i + 1)
	{
		int valAI = (float)analogRead(pinsArrayAI[i]) * 1000 / 1023;						// 10x Scale.  1023 from Hardware means 100,0%. i.e. Arduino analog input 977 ~ 95,5% inpput ~ Modbus:955 
		modbusTCPServer.inputRegisterWrite(i, valAI);

	}

	// Read Modbus Holding Registers (4x) and set the hardware output accordingly. 'Analog Output'
	for (int i = 0; i < sizeof(pinsArrayPWM); i = i + 1)
	{
		int valAO = (float)modbusTCPServer.holdingRegisterRead(i) * 1023 / 4000;			// 10x Scale.  1000 from Modbus means output 100,0%. i.e. Modbus:955 ~ 95,5% Output ~ Arduino analog write 244
		analogWrite(pinsArrayPWM[i], valAO);
	}

}