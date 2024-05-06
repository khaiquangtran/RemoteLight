/*  Three Wire LCD Interface using 74HC595*/

//Define 74HC595 Connections with msp430
const byte Clock= P1_0;
const byte Data=  P1_1;
const byte Latch= P1_2;

void LCDinit(void);
void LCDSendByte(char d,char RS);
void LCDgotoxy(unsigned char col, unsigned char row);
void LCDputs(const char* s);
void LCDclear(void);
void LCD2n(unsigned char x);
void ShiftData(char temp);
void LCD1n(unsigned char x);



//=============================================================
//             Send Data or Command to LCD
//=============================================================
void LCDSendByte(char d,char RS)
{
  char dH,dL,temp;
  //Keep Data on upper nybble
  dH = d & 0xF0;         //Get MSB
  dL = d & 0x0F;
  dL = d << 4;           //Get LSB
//Send MSB with E=clock  
  temp=0;
  temp=dH | RS | 0x02;  //MSB With RS+E bit
  ShiftData(temp);
//Send MSB with E=0
  temp=0;
  temp=dH | RS;  //MSB With RS bit
  ShiftData(temp);
//Send LSB with E=clock
  temp=0;
  temp=dL | RS | 0x02;  //MSB With RS+E bit
  ShiftData(temp);
//Send LSB with E=0
  temp=0;
  temp=dL | RS;  //MSB With RS bit
  ShiftData(temp);  
}
//=============================================
void ShiftData(char temp)
{
  int i;
  for(i=0;i<8;i++)      
  {
    if((temp & 0x80)==0x80)    //Send 1-bit to shift register
    {digitalWrite(Data,HIGH);}
    else
    {digitalWrite(Data,LOW);}
          
    digitalWrite(Clock,LOW);
    digitalWrite(Clock,HIGH);
    temp=temp<<1;
  }
  //Latch the data
  digitalWrite(Latch,LOW);
  delay(1);
  digitalWrite(Latch,HIGH);        
}
//=================================================================
//           LCD Display Initialization Function
//=================================================================
void LCDinit()
{
  pinMode(Data, OUTPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Latch, OUTPUT);
  
  int count;
  char t[]={0x43,0x03,0x03,0x02,0x28,0x01,0x0C,0x06,0x02,0x02};    
      for (count = 0; count <= 9; count++)
      {
        LCDSendByte(t[count],0);    //Command RS=0
      }
}
//=================================================================
//        Display Line on LCD at desired location Function
//=================================================================

void LCDgotoxy(unsigned char col, unsigned char row)
{
    unsigned char address;

    if(row!=0)
        address=0x40;
    else
        address=0;

    address += col;
    LCDSendByte(0x80|address,0);
    delayMicroseconds(2000);    
}
void LCDputs(const char* s)
{
    while(( s!=0) &&(*s !=0))
  {
    LCDSendByte(*s,1);
    s++;
  }
}
void LCDputc(char c)
{
      LCDSendByte(c,1);
}
void LCDclear(void)
{
    LCDSendByte(0x01,0); 
    LCDSendByte(0x02,0);        // display off
    delayMicroseconds(2000);   
}
void LCD2n(unsigned char x)
{
  LCDputc((x/10)+48);
  LCDputc((x%10)+48);
}
void LCD1n(unsigned char x)
{
  LCDputc((x%10)+48);
}
//=================================================================
