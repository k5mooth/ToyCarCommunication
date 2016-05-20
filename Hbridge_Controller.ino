int LeftMotorForward = 4; // Pin 10 has Left Motor connected on Arduino boards.
//int LeftMotorReverse = 9; // Pin 9 has Left Motor connected on Arduino boards.

int RightMotorForward = 2; // Pin 12 has Right Motor connected on Arduino boards.
//int RightMotorReverse = 13; // Pin 13 has Right Motor connected on Arduino boards.
int EnableMotorLeft = 10;
int EnableMotorRight = 9;


//-----------------------------------------------------------------------------  
void setup()
{
  pinMode(LeftMotorForward, OUTPUT);  // initialize the  pin as an output.
  pinMode(RightMotorForward, OUTPUT);  // initialize the  pin as an output.
  //pinMode(LeftMotorReverse, OUTPUT);  // initialize the  pin as an output.
  //pinMode(RightMotorReverse, OUTPUT);  // initialize the  pin as an output.
}
//-----------------------------------------------------------------------------  

// The following Loop is to make the Robot go staright
void loop()
{
 Driveforward(); 
 Rightturn();
 Leftturn();
 CheckEnable();
}
void Driveforward()
{  
   digitalWrite(RightMotorForward, HIGH);   // turn the Right Motor ON
   digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
   delay(2000);               // wait for  10 seconds
    Allstop();

}
//-----------------------------------------------------------------------------  
void Rightturn()
{ 
   digitalWrite(RightMotorForward, LOW);   // turn the Right Motor OFF
   digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
   delay(2000);               // wait for  10 seconds
    Allstop();

}
//-----------------------------------------------------------------------------  

void Leftturn()
{ 
   digitalWrite(RightMotorForward, HIGH);   // turn the Right Motor ON
   digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor OFF
   delay(2000);               // wait for  10 seconds
 
   Allstop();
}
//-----------------------------------------------------------------------------  
/*
void Reverse()
{ 
   digitalWrite(RightMotorReverse, HIGH);   // turn the Right Motor ON
   digitalWrite(LeftMotorReverse, HIGH);   // turn the Left Motor ON
   delay(2000);               // wait for a 10 seconds
 
   digitalWrite(RightMotorReverse, LOW);   // turn the Right Motor ON
   digitalWrite(LeftMotorReverse, LOW);   // turn the Left Motor ON
   delay(1000);               // wait for a 10 seconds
}
*/
//----------------------------------------------------------------------------- 
void Allstop()
{  
//   digitalWrite(RightMotorReverse, LOW);   // turn the Right Motor ON
//  digitalWrite(LeftMotorReverse, LOW);   // turn the Left Motor ON
//   delay(2000);               // wait for  10 seconds
  
   digitalWrite(RightMotorForward,LOW);   // turn the Right Motor OFF
   digitalWrite(LeftMotorForward, LOW);   // turn the Left Motor OFF
   delay(1000);    // wait for  10 seconds
}
//-----------------------------------------------------------------------------  

void CheckEnable()
{
 digitalWrite(RightMotorForward, HIGH);   // turn the Right Motor ON
   digitalWrite(LeftMotorForward, HIGH);   // turn the Left Motor ON
  for (int value =0; value < 255; value=value+50 ){
    analogWrite(EnableMotorLeft, value);
    delay(500);
    analogWrite(EnableMotorRight, value);
    delay(500);
  }
  
  Allstop();
}
