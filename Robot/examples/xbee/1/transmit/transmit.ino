/*This program initializes Serial port & sends a character capital “K” & then waits for a

     character   ‘O’ to switch ON  an LED at pin 13 for a short time */

char readChar;
void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(500);
  Serial.print('K');
  delay(500);

}

void loop()
{
  if (Serial.available() > 0) {

    readChar = Serial.read();
    if (readChar == 'O') {
      digitalWrite(13, HIGH);
      delay(5000);
      digitalWrite(13, LOW);

    }
  }
}
