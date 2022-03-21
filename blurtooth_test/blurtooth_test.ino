#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(115200);
  Serial.begin(115200);
 
  sendCommand("AT");
  sendCommand("AT+NAMEMyName");
  sendCommand("AT+BAUD8");
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

void loop() {

}
