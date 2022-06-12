
/*
  XBEE #2 SENDER
  ==============
  Button-Arduino-Xbee#2 > Xbee#1-Arduino-led
  Most code from : XBee Analog Duplex sender
  http://www.faludi.com/classes/sociableobjects/code/XBee_Analog_Duplex_Sender.pde
 */
void setup() {
  Serial.begin(9600);      
  
}

void loop() {
  Serial.print("ATRE\r");
  delay(500);
}

// Xbee configuration
void setDestination() {
  Serial.print("+++");
  char thisByte = 0;
  while (thisByte != '\r') { // wait for xbee response
    if (Serial.available() > 0) {
      thisByte = Serial.read(); 
    }
  }
  Serial.print("ATRE\r");
  Serial.print("ATDL0\r"); // sent to xbee 0
  Serial.print("ATMY1\r"); // this xbee is 1
  Serial.print("ATID1111\r");
  Serial.print("ATCN\r");
}

