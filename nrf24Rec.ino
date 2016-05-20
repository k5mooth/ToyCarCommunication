
/*
* Getting Started example sketch for nRF24L01+ radios
* This is an example of how to send data from one node to another using data structures
* Updated: Dec 2014 by TMRh20
*/
int EnableMotorLeft = 10;
int EnableMotorRight = 9;

int LeftMotorForward = 4;
int RightMotorForward = 2;

#include <SPI.h>
#include "RF24.h"

byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/


// Used to control whether this node is sending or receiving
bool role = 0;

/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
* See http://www.cplusplus.com/doc/tutorial/structures/
*/
struct dataStruct{
  unsigned long _micros;
  //int base_speed;
  int control_key;
  int value;
  
}ControlData;

struct dataStruct2{
  unsigned long _micros;
  float comm; 
}RespData;

int base_speed=200;
void setup() {

  pinMode(LeftMotorForward, OUTPUT);  // initialize the  pin as an output.
  pinMode(RightMotorForward, OUTPUT);  // initialize the  pin as an output.

  Serial.begin(9600);
  Serial.println(F("RF24/examples/GettingStarted_HandlingData"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }

  RespData.comm = 0.00;
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  Serial.flush();
  //radio.flush_tx();
  /*if (base_speed>0){
   digitalWrite(RightMotorForward, HIGH); // turn the Right Motor ON
   digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
  }
  else{
   digitalWrite(RightMotorForward, LOW); // turn the Right Motor ON
   digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor ON
  }
  */
  
  //analogWrite(EnableMotorLeft, base_speed);
  //analogWrite(EnableMotorRight, base_speed+20);
  if ( role == 0 )
  {
    
    if( radio.available()){
                                                           // Variable for the received timestamp
      while (radio.available()) {                          // While there is data ready
        radio.read( &ControlData, sizeof(ControlData) );             // Get the payload
      
      }
     
      radio.stopListening(); // First, stop listening so we can talk  
      //if (ControlData.control_key == 6 && ControlData.base_speed == prevBaseSpeed )
      //  return;
            
      parse_do(ControlData);
	 
      RespData.comm += 0.01;// Increment the float value
      RespData._micros=micros();
      //radio.write( &RespData, sizeof(RespData) );              // Send the final one back.      
      
      radio.startListening();                              // Now, resume listening so we catch the next packets.     
      
      Serial.print(F("Received Key:  "));
      Serial.print(ControlData.control_key);  
	  if (ControlData.control_key != 12 || ControlData.control_key != 13){
		Serial.print(F(" Value: "));
		Serial.print(ControlData.value);
	  }
	  Serial.println(F(""));

      Serial.print(F("Sent response "));
      Serial.print(RespData._micros);  
      Serial.print(F(" : "));
      Serial.println(RespData.comm);
      //delay(50);
   }
 }
}

void parse_do(struct dataStruct &RecData)
{
	//delay(50);
	switch(RecData.control_key){
		case 6: // speed control, PWM'ing the enable pin for both motors
		        break;
                         int oldBsp;
                         oldBsp=base_speed;
                        base_speed=RecData.value;
                        Serial.print("Changing Base from: ");
                        Serial.print(oldBsp);
                        Serial.print(" to: ");
                        Serial.println(RecData.value);
                        
                        analogWrite(EnableMotorLeft, RecData.value);
			analogWrite(EnableMotorRight, RecData.value);
			
			break;

		case 9: // turn left, lower speed of left wheel
			analogWrite(EnableMotorLeft, base_speed-RecData.value);
			break;

		case 10: // turn right, lower speed of right wheel
			analogWrite(EnableMotorRight, base_speed-RecData.value+20);
			break;

		case 12:
			digitalWrite(RightMotorForward, HIGH);   // turn the Right Motor OFF
			digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor OFF
			break;

		case 13:
			digitalWrite(RightMotorForward, LOW);   // turn the Right Motor OFF
			digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor OFF
			break;
	}
  //delay(50);
}
