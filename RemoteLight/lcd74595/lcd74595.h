#include "lcd74595.h"

class LCD74595
{
public:
  LCD74595(const byte ClockPin, const byte DataPin, const byte LatchPin);
  void LCDgotoxy(unsigned char col, unsigned char row);
  void LCDputs(const char *s);
  void LCDclear(void);
  void LCD2n(unsigned char x);
  void LCD1n(unsigned char x);

private:
  void LCDSendByte(char d, char RS);
  void ShiftData(char temp);
  void LCDinit(void);
  byte Clock;
  byte Data;
  byte Latch;
  
}