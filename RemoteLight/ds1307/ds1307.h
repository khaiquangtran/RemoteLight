#ifndef DS1307_H_
#define DS1307_H_

char *thu[]={" ","SUN","MON","TUE","WED","THU","FRI","SAT"};
class DS1307
{
public:
  DS1307(const byte SCL_pin,const byte SDA_pin);
  unsigned char DS_r(unsigned char addr);
  void DS_W(unsigned char addr, unsigned char dat);

private:
  void I2C_start(void);
  void I2C_stop(void);
  unsigned char I2C_w(unsigned char dat);
  unsigned char I2C_r(void);
  byte SCL;
  byte SDA;
};

#endif