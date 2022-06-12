/* This code waits  for a character capital ‘K’ over Serial port & switches ON a buzzer

     connected to pin 11 for 1.5 secs..Then it sends a character capital ‘O’ over Serial

      to light up an LED at the Sender side Arduino A.  */


int Buzzer=11; 
void setup() 
{ 
  pinMode(Buzzer,OUTPUT); 
  Serial.begin(9600); 
}

void loop() 
{ 
  if(Serial.available()>0){ 
       if(Serial.read() == 'K'){ 
      digitalWrite(Buzzer,HIGH); 
      delay(3000); 
      digitalWrite(Buzzer,LOW);

     delay(1000);

     Serial.print('O'); 
       } 
    } 
}
