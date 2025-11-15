#ifndef SIMPLEKEYPAD_H
#define SIMPLEKEYPAD_H

#include <Arduino.h>

class SimpleKeypad {
  public:
    SimpleKeypad(char *keyChars, byte *rowPins, byte *colPins, byte nbRows, byte nbCols);
    char scan();
    char getKey();
    char currentKeys[2] = {0};

  private:
    char *keyChars;
    byte *rowPins;
    byte *colPins;
    byte nbRows;
    byte nbCols;
    static const int debounceTime = 10;
    unsigned long debounceTimer = 0;
    static const int scanPeriod = 2;
    unsigned long scanTimer = 0;
    char currentKey = 0;
    char lastKey = 0;
    bool hold = 0;
};

#endif
