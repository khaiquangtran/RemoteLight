#ifndef LCD74595_H_
#define LCD74595_H_
class LCD74595
{
public:
  LCD74595(const byte ClockPin, const byte DataPin, const byte LatchPin);
  void LCDgotoxy(unsigned char col, unsigned char row);
  void LCDputs(const char *s);
  void LCDclear(void);
  void LCD2n(unsigned char x);
  void LCD1n(unsigned char x);
  void LCDinit(void);

private:
  void LCDSendByte(char d, char RS);
  void ShiftData(char temp);
  void LCDputc(char c);

  byte Clock;
  byte Data;
  byte Latch;
 };

 #endif