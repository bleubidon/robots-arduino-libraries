#define encoderPin1 11
#define encoderPin2 12

unsigned int lastEncoded = 0;
long encoderValue = 0;

void setup()
{
    Serial.begin(9600);

    pinMode(encoderPin1, INPUT_PULLUP);
    pinMode(encoderPin2, INPUT_PULLUP);
}

void loop()
{
    updateEncoder();
    Serial.print("x= ");
    Serial.println(encoderValue);
}

void updateEncoder()
{
    unsigned int MSb = digitalRead(encoderPin1);  // MSb == most significant bit
    unsigned int LSb = digitalRead(encoderPin2);  // LSb == least significant bit

    unsigned int encoded = (MSb << 1) | LSb;
    unsigned int sum = (lastEncoded << 2) | encoded;  // adding it to the previous encoded value
    
    if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    {
        encoderValue ++;
    }
    else if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    {
        encoderValue --;
    }
       
    lastEncoded = encoded;
}

