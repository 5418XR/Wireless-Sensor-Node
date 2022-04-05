#include "SoftwareSerial.h"

SoftwareSerial mySerial(10, 11); // TX, RX

void initBluetooth(){
  // put your setup code here, to run once:
  mySerial.begin(9600);
  //Serial.begin(9600);
 
  sendCommand("AT");
  sendCommand("AT+NAMEMyName");
  sendCommand("AT+BAUD0");
  sendCommand("AT+VERSION");
  sendCommand("AT+LADDR");
  sendCommand("AT+BAUD");
  sendCommand("AT+PIN1234");
  //sendCommand("AT+DISC");
}

void sendCommand(const char * command){
  Serial.print("Command send :");
  Serial.println(command);
  mySerial.println(command);
  //wait some time
  delay(1000);
 
  char reply[1000];
  int i = 0;
  while (mySerial.available()) {
    reply[i] = mySerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("Reply end");
}

// Used for manual setup
void updateSerial() {
  if (mySerial.available())       
   Serial.write(mySerial.read()); 

  if (Serial.available())     
    mySerial.write(Serial.read());
}
