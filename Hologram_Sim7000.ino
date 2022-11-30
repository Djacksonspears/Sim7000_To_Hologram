#include <SoftwareSerial.h>
#include "millisDelay.h"

SoftwareSerial serialSIM7000(10, 11);

//VARS
char Message[120];
byte numChars = 255;
int sensorValue = 0;

//TIMER VARS
millisDelay CFUN, CNMP, CMNB, CIPSTATUS1, CIPSHUT, CIPSTATUS2, CSTT, CIICR, CIFSR, CIPSTART, CIPSEND, DATA, CHAR, CGNSPWRON, CGNSINF, CGNSPWROFF, REFRESHTIMER;

//TIMER ARRAYS
millisDelay hologramTimers[] = {CFUN, CNMP, CMNB, CIPSTATUS1, CIPSHUT, CIPSTATUS2, CSTT, CIICR, CIFSR, CIPSTART, CIPSEND, DATA};

//TIMER DELAY VAR
unsigned long timerDuration = 1;
unsigned long refreshRate = 45000;

//AT COMMAND ARRAYS
char *hologramCommands[] = {"AT+CFUN=1\r\n", "AT+CNMP=38\r\n", "AT+CMNB=1\r\n", "AT+CIPSTATUS\r\n", "AT+CIPSHUT\r\n", "AT+CIPSTATUS\r\n", "AT+CSTT=\"hologram\"\r\n", "AT+CIICR\r\n", "AT+CIFSR\r\n", "AT+CIPSTART=\"TCP\",\"cloudsocket.hologram.io\",\"9999\"\r\n", "AT+CIPSEND\r\n", Message};

void setup()
{
  Serial.begin(9600);
  serialSIM7000.begin(9600);			   
}

void loop()
{
  
  //STARTS THE LOOP DELAY TIMER
  REFRESHTIMER.start(refreshRate);
  
  //WAITS FOR THE DELAY TO FINISH BEFORE EXECUTING
  while(!REFRESHTIMER.justFinished()){}
  
  //FORMATS MESSAGE AND SENDS TO HOLOGRAM
  sprintf(Message, "{\"k\":\"G951+L*L\",\"d\":\"%d\",\"t\":\"Topic\"}\r\n", sensorValue);
  
  Serial.println("SENDING DATA TO HOLOGRAM..");
  
  //METHOD TO SEND TO HOLOGRAM
  SendToHologram();

  Serial.println("SUCCESS ON SENDING HOLOGRAM DATA..");
  
  //RESTARTS THE LOOP DELAY TIMER
  REFRESHTIMER.start(refreshRate);
}

//LOOPS OVER ARRAYS TO SEND DATA TO HOLOGRAM.IO
void SendToHologram()
{
  for ( int i = 0; i < 12; ++i )
  {
    hologramTimers[i].start(timerDuration);
    while (!hologramTimers[i].justFinished()){}
    serialSIM7000.write(hologramCommands[i]);
    Serial.println(serialSIM7000.readString());
  }
  serialSIM7000.write((char)26);
}

