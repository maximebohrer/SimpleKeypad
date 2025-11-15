#include "SimpleKeypad.h"

SimpleKeypad::SimpleKeypad(char *keyChars, byte *rowPins, byte *colPins, byte nbRows, byte nbCols) {
  this->keyChars = keyChars;
  this->rowPins = rowPins;
  this->colPins = colPins;
  this->nbRows = nbRows;
  this->nbCols = nbCols;

  for (byte r = 0; r < nbRows; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }
  for (byte c = 0; c < nbCols; c++) {
    digitalWrite(colPins[c], HIGH);
    pinMode(colPins[c], INPUT);
  }
}

char SimpleKeypad::scan() {
  // Scan of all pressed keys (max 2). Due to matrix keypad wiring, more than two simultaneous key presses cannot be reliably identified.
  char pressedKeys[2] = { 0 };
  int count = 0;
  for (byte c = 0; c < nbCols; c++) {
    pinMode(colPins[c], OUTPUT);  // Each column is set to a low output one after the other.
    digitalWrite(colPins[c], LOW);
    for (byte l = 0; l < nbRows; l++) {
      if (!digitalRead(rowPins[l])) {                                   // For each column we can then test if any line is low, which would mean that the key at these speciic row and column is being pressed.
        if (count < 2) pressedKeys[count] = keyChars[l * nbCols + c];   // First key found is placed in pressedKeys[0], second key found is placed in pressedKeys[1].
        count++;
      }
    }
    digitalWrite(colPins[c], HIGH);
    pinMode(colPins[c], INPUT);  // This ensures that only one pin is an output at a time, to prevent low and high output from being shorted together if multiple keys are pressed at the same time.
  }

  // The detection of 2 keys helps improve the user experience by registering a key press even if the last one was not fully released when typing quickly.
  // Here we deternine which key press to register (the most recent).
  if (count <= 2) {
    if (pressedKeys[0] && pressedKeys[0] != currentKeys[0] && pressedKeys[0] != currentKeys[1]) currentKey = pressedKeys[0];        // If pressedKey[0] is a new key, return this one.
    else if (pressedKeys[1] && pressedKeys[1] != currentKeys[0] && pressedKeys[1] != currentKeys[1]) currentKey = pressedKeys[1];   // If pressedKey[1] is a new key, return this one.
    else if (!count) currentKey = 0;
    currentKeys[0] = pressedKeys[0];
    currentKeys[1] = pressedKeys[1];
  }
  return currentKey;
}

char SimpleKeypad::getKey() {
  // To prevent slowing down the code too much, the scan function is only executed every 2 ms.
  if (millis() - scanTimer <= scanPeriod) {
    return 0;
  }
  scanTimer = millis();
  char key = scan();

  // Debounce and return key only once when it changes
  if (key != lastKey) {
    lastKey = key;
    debounceTimer = millis();
    hold = false;
  } else if (key && !hold && millis() - debounceTimer > debounceTime) {
    hold = true;
    return key;
  }
  return 0;
}
