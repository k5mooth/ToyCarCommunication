/*
 PROJECT: ArduDroid 
 PROGRAMMER: Hazim Bitar (techbitar at gmail dot com)
 DATE: Oct 31, 2013
 FILE: ardudroid.ino
 LICENSE: Public domain
*/

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2

int EnableMotorLeft = 10;
int EnableMotorRight = 9;

int LeftMotorForward = 4;
int RightMotorForward = 2;

#include <SPI.h>
#include <RF24.h>

byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this r1adio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/


// Used to control whether this node is sending or receiving
bool role = 1;

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
boolean go=false;
String inText;
void setup() {

  //pinMode(5,OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);  // initialize the  pin as an output.
  pinMode(RightMotorForward, OUTPUT);  // initialize the  pin as an output.
  Serial.begin(9600);
  Serial.flush();
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
  
  base_speed = 200;
  // Start the radio listening for data
  radio.startListening();
}


void loop()
{
  
  Serial.flush();
  int ard_command = 0;
  int pin_num = 0;
  int pin_value = 0;

  char get_char = ' ';  //read serial
  radio.stopListening();
  // wait for incoming data
  if (go){
   digitalWrite(RightMotorForward, HIGH); // turn the Right Motor ON
   digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
  }else{
   digitalWrite(RightMotorForward, LOW); // turn the Right Motor ON
   digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor ON
  }
  
  if (Serial.available() < 1) return; // if serial empty, return to loop().

  // parse incoming command start flag 
  get_char = Serial.read();
  if (get_char != START_CMD_CHAR) return; // if no command start flag, return to loop().

  // parse incoming command type
  ard_command = Serial.parseInt(); // read the command
  
  // parse incoming pin# and value  
  pin_num = Serial.parseInt(); // read the pin
  pin_value = Serial.parseInt();  // read the value

  // 1) GET TEXT COMMAND FROM ARDUDROID
  if (ard_command == CMD_TEXT){   
    inText =""; //clears variable for new input   
    while (Serial.available())  {
      char c = Serial.read();  //gets one byte from serial buffer
      delay(5);
      if (c == END_CMD_CHAR) { // if we the complete string has been read
        // add your code here
        break;
      }              
      else {
        if (c !=  DIV_CMD_CHAR) {
          inText += c; 
          delay(5);
        }
      }
    }
  }

  
  // 2) GET digitalWrite DATA FROM ARDUDROID
  if (ard_command == CMD_DIGITALWRITE){  
    if (pin_value == PIN_LOW) pin_value = LOW;
    else if (pin_value == PIN_HIGH) pin_value = HIGH;
    else return; // error in pin value. return. 
    set_digitalwrite( pin_num,  pin_value);  // Uncomment this function if you wish to use 
    return;  // return from start of loop()
  }

  // 3) GET analogWrite DATA FROM ARDUDROID
  if (ard_command == CMD_ANALOGWRITE) {  
    //analogWrite(  pin_num, pin_value ); 
    // add your code here

	if (pin_num == 6 || pin_num == 9 || pin_num == 10){
          Serial.print("BaseSpeed: ");
          Serial.println(base_speed);
    	  motor_control(pin_num, pin_value);  
          }
    return;  // Done. return to loop();
  }

  // 4) SEND DATA TO ARDUDROID
  if (ard_command == CMD_READ_ARDUDROID) { 
    // char send_to_android[] = "Place your text here." ;
    // Serial.println(send_to_android);   // Example: Sending text
    
     return;  // Done. return to loop();
  }
}

// 2a) select the requested pin# for DigitalWrite action
void set_digitalwrite(int pin_num, int pin_value)
{
  switch (pin_num) {
  case 13:
    //pinMode(13, OUTPUT);
    //digitalWrite(13, pin_value);   
    go=false;
     //digitalWrite(RightMotorForward, LOW);   // turn the Right Motor OFF
     //digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor OFF
	ControlData.control_key= 13;
	send_wait_for_resp(ControlData);
    break;
  case 12:
    //pinMode(12, OUTPUT);
    //digitalWrite(12, pin_value);   
    go=true;
    //digitalWrite(RightMotorForward, HIGH);   // turn the Right Motor ON
   //digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
	ControlData.control_key= 12;
	send_wait_for_resp(ControlData);
    break;
  case 11:
    pinMode(11, OUTPUT);
    digitalWrite(11, pin_value);         
    // add your code here 
    break;
  case 10:
    //pinMode(10, OUTPUT);
    //digitalWrite(10, pin_value);         
    ControlData.control_key=10;
    ControlData.value=0;
    analogWrite(RightMotorForward,base_speed);

    send_wait_for_resp(ControlData);

    // add your code here 
    break;
  case 9:
    //pinMode(9, OUTPUT);
    //digitalWrite(9, pin_value);         
    ControlData.control_key=9;
    ControlData.value=0;
    analogWrite(LeftMotorForward,base_speed);
    send_wait_for_resp(ControlData);
    // add your code here 
    break;
  case 8:
    pinMode(8, OUTPUT);
    digitalWrite(8, pin_value);         
    // add your code here 
    break;
  case 7:
    pinMode(7, OUTPUT);
    digitalWrite(7, pin_value);         
    // add your code here 
    break;
  case 6:
    pinMode(6, OUTPUT);
    digitalWrite(6, pin_value);         
    
  case 5:
    pinMode(5, OUTPUT);
    digitalWrite(5, pin_value);         
    break;
  case 4:
    pinMode(4, OUTPUT);
    digitalWrite(4, pin_value);         
    // add your code here 
    break;
  case 3:
    pinMode(3, OUTPUT);
    digitalWrite(3, pin_value);         
    // add your code here 
    break;
  case 2:
    pinMode(2, OUTPUT);
    digitalWrite(2, pin_value); 
    // add your code here       
    break;      
    // default: 
    // if nothing else matches, do the default
    // default is optional
  } 

}

void motor_control(int control_key, int value)
{
 /* 
  * Motor control using the L293D (H-bridge)	
  *
  */
	//int count =0; 
        radio.flush_tx();
	switch(control_key){
		case 6: // speed control, PWM'ing the enable pin for both motors
		break;
                    int oldBsp;
                         oldBsp=base_speed;
                        base_speed=value;
                        ControlData.value=value;
			ControlData.control_key=6;
                        Serial.print("Changing Base from: ");
                        Serial.print(oldBsp);
                        Serial.print(" to: ");
                        Serial.println(value);
                        
			analogWrite(EnableMotorLeft, value);
			analogWrite(EnableMotorRight, value);
                    
                        Serial.print("Changed my Speed, ");
			ControlData._micros = micros();
                        Serial.println("Gathered Micros");
                        send_wait_for_resp(ControlData);
			
			break;

		case 9: // turn left, lower speed of left wheel
			ControlData.control_key=9;
			ControlData.value= value;
			analogWrite(EnableMotorLeft, base_speed-value);
			
                        ControlData._micros = micros();
                        send_wait_for_resp(ControlData);

			break;

		case 10: // turn right, lower speed of right wheel
			ControlData.control_key=10;
			ControlData.value= value;
			analogWrite(EnableMotorRight, base_speed-value);
			
                        ControlData._micros = micros();
                        send_wait_for_resp(ControlData);

			break;
                default:
                  
                  break;

	}
//delay(50);

}

void send_wait_for_resp(struct dataStruct &ControlData)
{
  //delay(50);
  if (!radio.write( &ControlData, sizeof(ControlData) )){
       Serial.println(F("failed"));
        //pinMode(5,OUTPUT);
        //delay(5);
        //digitalWrite(5,HIGH);
        //send_wait_for_resp(ControlData);
        //return ;
     }

	//radio.startListening();                                    // Now, continue listening-
	Serial.print(F("Sent Key: "));
	Serial.print(ControlData.control_key);
	if (ControlData.control_key != 12 || ControlData.control_key != 13){
		Serial.print(F(" Value: "));
		Serial.print(ControlData.value);

	}
	Serial.println(F(" "));
	/*
	unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
	boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
				
	while ( ! radio.available() ){                             // While nothing is received
		if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			timeout = true;
			break;
		}      
	}
			
	if ( timeout ){    // Describe the results
		Serial.println(F("Failed, response timed out."));
	}else{ // Grab the response, compare, and send to debugging spew
		  radio.read( &RespData, sizeof(RespData) );
		  unsigned long time = micros();
		  Serial.print(RespData._micros);
		  Serial.print(F(", Round-trip delay "));
		  Serial.print(time-RespData._micros);
		  Serial.print(F(" Comm Value "));
		  Serial.println(RespData.comm);
	}

  delay(500);
  */
}
