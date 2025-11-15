SimpleKeypad library for Arduino
================================

SimpleKeypad is an Arduino library for using matrix keypads.
SimpleKeypad is based on the [original Keypad library](https://github.com/Chris--A/Keypad), but it is a simplified version. It does not support mutiple keypresses. It requires less memory and runs a bit faster.

This code handles up to two keypress. Detecting two keys enables to use to detect a key press even if the previous key was not fully released. Some clever logic determines which of the two was pressed last. This makes the keypad less frustrating to use, and makes it easier to type quickly.
Trying to detect more than two is pointless because ghosting can occur if three keys or more are pressed, meaning that an unpressed key can be registered as pressed (unless diodes are used in the circuit).

This code was tested on the Arduino Uno and Nano, but it should work on any card.

Functions
---------

`char SimpleKeypad::SimpleKeypad()`: Constructor (see the example).
`char SimpleKeypad::getKey()`: Scans the keypad every and returns a key only one time, when you start pressing it. Does some debouncing.
`SimpleKeypad::scan()`: Scans the keypad and returns the raw output (the key currently being held down).
`SimpleKeypad::currentKeys`: After calling `getKey` or `scan`, the two keys that are pressed are stored in this array of two chars.

Example
-------

```c++
#include <SimpleKeypad.h>

const byte nb_rows = 4;                         // four rows
const byte nb_cols = 4;                         // four columns
char key_chars[nb_rows][nb_cols] = {            // The symbols of the keys
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[nb_rows] = {9, 8, 7, 6};           // The pins where the rows are connected
byte colPins[nb_cols] = {5, 4, 3, 2};           // The pins where the columns are connected

SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);   // New keypad called kp1

void setup() {
  Serial.begin(9600);
  Serial.println("Press any key on the keypad and it will show up here:");
}

void loop() {
  char key = kp1.getKey();                      // The getKey function scans the keypad every 2 ms and returns a key only one time, when you start pressing it
  if (key) {                                    // If getKey returned any key
    Serial.println(key);                        // it is printed on the serial monitor
  }
}
```
