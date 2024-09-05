#include <PAJ7620.h>

/**
 * @author  Yayi
 * @Website rtrobot.org
 * @Version 1.0.0
 * @Copyright	This code is public domain but you buy me a beer if you use
 * this and we meet someday (Beerware license).
 */
#include <Wire.h>
#include "PAJ7620.h"
#include <Arduino.h>
#define PAJ7620_Delay	800

void setup()
{
	uint8_t error = 0;

	Serial.begin(115200);
	Serial.println("\r\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

	error = PAJ7620Init();			// initialize Paj7620 registers
	if (error) 
	{
		Serial.print("INIT ERROR,CODE:");
		Serial.println(error);
	}
	else
	{
		Serial.println("INIT OK");
	}
	Serial.println("Please input your gestures:\n");
}

void loop()
{
	uint8_t data = 0;
	
	PAJ7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
	switch (data)								// When different gestures be detected, the variable 'data' will be set to different values by PAJ7620ReadReg(0x43, 1, &data).
	{
		case GES_RIGHT_FLAG:
			delay(PAJ7620_Delay);
			PAJ7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
				Serial.println("Forward");
				delay(PAJ7620_Delay);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
				Serial.println("Backward");
				delay(PAJ7620_Delay);
			}
			else
				Serial.println("Right");        
			break;
			
		case GES_LEFT_FLAG: 
			delay(PAJ7620_Delay);
			PAJ7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
				Serial.println("Forward");
				delay(PAJ7620_Delay);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
				Serial.println("Backward");
				delay(PAJ7620_Delay);
			}
			else
				Serial.println("Left");       
		break;
		
		case GES_UP_FLAG:
			delay(PAJ7620_Delay);
			PAJ7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
				Serial.println("Forward");
				delay(PAJ7620_Delay);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
				Serial.println("Backward");
				delay(PAJ7620_Delay);
			}
			else
				Serial.println("Up");        
			break;
			
		case GES_DOWN_FLAG:
			delay(PAJ7620_Delay);
			PAJ7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
				Serial.println("Forward");
				delay(PAJ7620_Delay);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
				Serial.println("Backward");
				delay(PAJ7620_Delay);
			}
			else
				Serial.println("Down");          
		break;
		
		case GES_FORWARD_FLAG:
			Serial.println("Forward");
			delay(PAJ7620_Delay);
		break;
			
		case GES_BACKWARD_FLAG:		  
			Serial.println("Backward");
			delay(PAJ7620_Delay);
		break;
			
		case GES_CLOCKWISE_FLAG:
			Serial.println("Clockwise");
		break;
			
		case GES_COUNT_CLOCKWISE_FLAG:
			Serial.println("anti-clockwise");
		break;  
			
		default:
			PAJ7620ReadReg(0x44, 1, &data);
			if (data == GES_WAVE_FLAG) 
				Serial.println("wave");
		break;
	}
	delay(100);
}

