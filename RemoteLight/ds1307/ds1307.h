class DS1307_I2C
{
public:
  DS1307_I2C(const byte SCL_pin,const byte SDA_pin);
  unsigned char DS_r(unsigned char addr);
  void DS_W(unsigned char addr, unsigned char dat);

private:
  void I2C_start(void);
  void I2C_stop(void);
  unsigned char I2C_w(unsigned char dat);
  unsigned char I2C_r(void);
  char *thu[] = {" ", "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  byte SCL;
  byte SDA;
}
