#include <string.h>
#include "board.h"
#include "radio.h"
#include "LoRaMac.h"

// Button defenition
#define BUTTON_1 PB_15
Gpio_t Button1;

// Mote device IEEE EUI
static uint8_t DevEui[] = { 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// AES encryption/decryption cipher network session key
static uint8_t NwkSKey[] = 
{ 
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

// AES encryption/decryption cipher application session key
static uint8_t AppSKey[] = 
{ 
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

// Device address
static uint32_t DevAddr = 0x8B188E;

// Port
static uint8_t AppPort = 3;

void ResetLEDs()
{
		GpioWrite(&Led2, 0);
		GpioWrite(&Led3, 0);
		GpioWrite(&Led4, 0);
}

void SendToGateway()
{
		ResetLEDs();	
	
		char* data = "Hello, World!";
		uint8_t result = LoRaMacSendFrame(AppPort, data, strlen(data));
		
		// Everything is fine
		if(result==0) {	GpioWrite(&Led3, 1); }
		// No free channel
		else if(result==5) { GpioWrite(&Led2, 1); }	
		// Some other error
		else { GpioWrite(&Led4, 1); }	
}

void BoardSetup()
{
		BoardInitMcu();
		BoardInitPeriph();
		BoardGetUniqueId(DevEui);
}

void LoRaMacSetup()
{
		LoRaMacInit(NULL);
		LoRaMacInitNwkIds(0x000000, DevAddr, NwkSKey, AppSKey);
		LoRaMacTestRxWindowsOn(false);
		LoRaMacSetDeviceClass(CLASS_C);	
	
		// Disable duty cycle for TESTING ONLY!!!
		// LoRaMacSetDutyCycleOn(false);		
}

void ButtonSetup()
{
		GpioInit(&Button1, BUTTON_1, PIN_INPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 1);
		GpioSetInterrupt(&Button1, IRQ_RISING_EDGE, IRQ_HIGH_PRIORITY, SendToGateway);
}

int main( void )
{
		BoardSetup();
		LoRaMacSetup();		
		ButtonSetup();		
		while(1);    
}
