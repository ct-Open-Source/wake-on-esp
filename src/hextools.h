#include <Arduino.h>

byte valFromChar(char c) {
  if(c >= 'a' && c <= 'f') return (byte( (c - 'a') + 10) & 0x0F);
  if(c >= 'A' && c <= 'F') return (byte( (c - 'A') + 10) & 0x0F);
  if(c >= '0' && c <= '9') return (byte( (c - '0')) & 0x0F);
  return 0;
}

/*
* Very simple converter from a String representation of a MAC address to
* 6 bytes. Does not handle errors or delimiters, but requires very little
* code space and no libraries.
*/
void StringToBytes(const String input, byte *bytes) {
  if(input.length() >= 12) {
    for(unsigned int i = 0; i < 6; i++) {
      bytes[i] = byte((valFromChar(input.charAt(i*2)) << 4) | valFromChar(input.charAt(i*2 + 1)));
    }
  } else {
    Serial.println("Incorrect MAC format.");
  }
}

