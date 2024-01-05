
/*
  XBEE #1 RECEIVER
  ================
  Button-Arduino-Xbee#2 > Xbee#1-Arduino-led
  Most code from : XBee Analog Duplex sender
  http://www.faludi.com/classes/sociableobjects/code/XBee_Analog_Duplex_Sender.pde
 */
 
const int led = 13;

void setup() {
  Serial.begin(9600);      
  pinMode (led, OUTPUT);
  
  Serial.print("+++");
  char thisByte = 0;
  while (thisByte != '\r') {
    if (Serial.available() > 0) {
      thisByte = Serial.read();
      Serial.println(thisByte);
    }
  }
  
  blink(led, 3);
}

void loop() {
}

// Blink 
void blink(int thisPin, int howManyTimes) {
  for (int blinks=0; blinks< howManyTimes; blinks++) {
    digitalWrite(thisPin, HIGH);
    delay(200);
    digitalWrite(thisPin, LOW);
    delay(200);  
  }
}


