/*
This program is intended to be run on an ATmega2560.
It allows reading inputs from numerous pushbuttons, and repeateadly sending the buttons states over a serial link.
This can prove useful in a scenario where a chip that would read from this serial link doesn't itself have enough IO to read states from all buttons. Then the ATmega2560 would serve as an intermediary device.
*/

#define NUM_BUTTONS  34
#define ARDUI_SERIAL Serial3
#define DEBUG true

const unsigned int buttonsPins[NUM_BUTTONS] = {44, 42, 40, 38, 36, 52, 50, 48, 46, 53, 34, 32, 51, 49, 47, 45, 43, 30, 28, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 26, 24, 22, 21, 20};
struct Buttons {
    unsigned long buttonsPart1;  // buttons 1 to 32
    byte buttonsPart2;  // buttons 33 and 34
};

Buttons pressedButtons = {0, 0};
Buttons previousButtons = {0, 0};

int readButtonStateFromStruct(Buttons *buttons, int buttonIndex)
{
    if (buttonIndex < 32)
        return bitRead(buttons->buttonsPart1, buttonIndex);

    return bitRead(buttons->buttonsPart2, buttonIndex - 32);
}

void writeButtonStateToStruct(Buttons *buttons, int buttonIndex)
{
    if (buttonIndex < 32)
        bitWrite(buttons->buttonsPart1, buttonIndex, !digitalRead(buttonsPins[buttonIndex]));
    else
        bitWrite(buttons->buttonsPart2, buttonIndex - 32, !digitalRead(buttonsPins[buttonIndex]));
}

void setup() {
    if (DEBUG)
        Serial.begin(115200);
    ARDUI_SERIAL.begin(9600);
    for (int _ = 0; _ < NUM_BUTTONS; _++)
        pinMode(buttonsPins[_], INPUT_PULLUP);
}

void loop() {
    if (DEBUG)
    {
        if (ARDUI_SERIAL.available() > 0) {
            int incomingByte = ARDUI_SERIAL.read();
            Serial.write(incomingByte);
        }
    }

    // Read all buttons
    for (int _ = 0; _ < NUM_BUTTONS; _++)
        writeButtonStateToStruct(&pressedButtons, _);

        // ARDUI_SERIAL.print("\nbuttons: ");
    // for (int _ = 0; _ < NUM_BUTTONS; _++)
        // ARDUI_SERIAL.print(getButtonState(&pressedButtons, _));

    // Determine for which buttons the status changed and send the changed status over Serial
    for (int _ = 0; _ < NUM_BUTTONS; _++)
    {
        if (readButtonStateFromStruct(&pressedButtons, _) != readButtonStateFromStruct(&previousButtons, _))
        {
            if (_ < 10)
                ARDUI_SERIAL.print("0");
            ARDUI_SERIAL.print(_);
            ARDUI_SERIAL.print(readButtonStateFromStruct(&pressedButtons, _));
            ARDUI_SERIAL.print(";");

            writeButtonStateToStruct(&previousButtons, _);
        }
    }
}

