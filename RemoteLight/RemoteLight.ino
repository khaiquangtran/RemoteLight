#include "ds1307/TypeDef.h"
#include "lcd74595/lcd74595.h"
#include "ds1307/ds1307.h"
#include "IRremote/IRremote.h"
#include "IRremote/IRremote.cpp"

const byte receiverPin = P1_5;
IRrecv irrecv(receiverPin);
decode_results results;

void read_DS();
void show_DS();
void cai_dat_thoi_gian();

char sec, h24, min, day, dt, mth, y;
char _ud = 2;
char _selt = 0;
void set_time(unsigned char S1, unsigned char S2, unsigned char PH1, unsigned char PH2, unsigned char GIO1, unsigned char GIO2, unsigned char SW1, unsigned char SW2, char n);
void chon_den_hen_gio();
void onoff();
unsigned long i = 0;
bool state1 = 1;
bool state2 = 1;
bool state3 = 1;
bool state4 = 1;
bool state5 = 1;
void onoff1()
{
  i = 0;
  detachInterrupt(digitalPinToInterrupt(DEN11));
  state1 = !state1;
  digitalWrite(DEN1, state1);
  while (i < 800010)
    i++;

  attachInterrupt(digitalPinToInterrupt(DEN11), onoff1, FALLING);
}
void onoff2()
{
  i = 0;
  detachInterrupt(digitalPinToInterrupt(DEN21));
  state2 = !state2;
  digitalWrite(DEN2, state2);
  while (i < 800010)
    i++;
  attachInterrupt(digitalPinToInterrupt(DEN21), onoff2, FALLING);
}
void onoff3()
{
  i = 0;
  detachInterrupt(digitalPinToInterrupt(DEN31));
  state3 = !state3;
  digitalWrite(DEN3, state3);
  while (i < 800010)
    i++;

  attachInterrupt(digitalPinToInterrupt(DEN31), onoff3, FALLING);
}

char preSec;
char preMin;
char preH24;
char tamp_day;
char previousDay;
char tamp_sec;
char previousSec;
char flag;
void setup()
{
  // put your setup code here, to run once:
  pinMode(DEN1, OUTPUT);
  pinMode(DEN2, OUTPUT);
  pinMode(DEN3, OUTPUT);
  pinMode(DEN4, OUTPUT);
  pinMode(DENLCD, OUTPUT);

  digitalWrite(DEN1, HIGH);
  digitalWrite(DEN2, HIGH);
  digitalWrite(DEN3, HIGH);
  digitalWrite(DEN4, HIGH);
  digitalWrite(DENLCD, HIGH);
  irrecv.enableIRIn();
  LCDinit();
  LCDgotoxy(0, 0);
  LCDputs("    WELCOME     ");
  LCDgotoxy(0, 1);
  LCDputs("KHAI'S EQUIPMENT");
  delay(1500);
  digitalWrite(DENLCD, LOW);
  LCDclear();
  pinMode(DEN11, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN11), onoff1, FALLING);
  pinMode(DEN21, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN21), onoff2, FALLING);
  pinMode(DEN31, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN31), onoff3, FALLING);

  preSec = DS_r(Pre_Sec);
  preMin = DS_r(Pre_Min);
  preH24 = DS_r(Pre_H24);
  tamp_day = DS_r(Tampday);
  previousDay = DS_r(DAY);

  previousSec = DS_r(SEC);
  tamp_sec = 0;
  flag = 1;
  /*DS_W(0x00,0); //sec
  DS_W(0x01,0);  //min
  DS_W(0x02,4);  //h24
  DS_W(0x03,7);  //dat
  DS_W(0x04,23);  //date
  DS_W(0x05,7); //moth
  DS_W(0x06,22); //year*/
}
void loop()
{
  // put your main code here, to run repeatedly:

  read_DS();
  if (flag == 1)
    show_DS();
  onoff();
  if (irrecv.decode(&results)) // náº¿u nháº­n Ä‘Æ°á»£c tÃ­n hiá»‡u
  {
    irrecv.resume();       // nháº­n giÃ¡ trá»‹ tiáº¿p theo
    switch (results.value) // phim App
    {
    case 0xFF6A95:
    { // phim App
      cai_dat_thoi_gian();
      break;
    }
    case 0xFFB24D:
    { // phim menu
      chon_den_hen_gio();
      break;
    }
    case 0xFF807F:
    { // phim 1
      state1 = !state1;
      digitalWrite(DEN1, state1);
      break;
    }
    case 0xFF40BF:
    { // phim 2
      state2 = !state2;
      digitalWrite(DEN2, state2);
      break;
    }
    case 0xFFC03F:
    { // phim 3
      state3 = !state3;
      digitalWrite(DEN3, state3);
      break;
    }
    case 0xFF20DF:
    { // phim 4
      state4 = !state4;
      digitalWrite(DEN4, state4);
      break;
    }
    case 0xFFA05F:
    { // phim 5
      state5 = !state5;
      digitalWrite(DENLCD, state5);
      break;
    }
    case 0xFF609F:
    { // phim 5
      {
        tamp_sec = 0;
        flag = 1;
        break;
      }
    }
    default:
      break;
    }
  }
  if ((previousDay > day || previousDay < day) && tamp_day < 4)
  {
    tamp_day++;
    DS_W(Tampday, tamp_day);
    previousDay = day;
  }
  if (tamp_day == 4 && preSec == sec && preMin == min && preH24 == h24)
  {
    sec = sec - 11; // tru di thoi tang hay giam
    DS_W(SEC, sec);
    tamp_day = 0;
    DS_W(Tampday, tamp_day);
  }
  if ((previousSec > sec || previousSec < sec) && tamp_sec < 30)
  {
    tamp_sec++;
    previousSec = sec;
  }
  if (tamp_sec == 30)
  {
    flag = 0;
    tamp_sec = 31;
    LCDclear();
  }
}

void show_DS()
{
  LCDgotoxy(0, 0);
  LCDputs(day[thu]);
  LCDgotoxy(3, 0);
  LCDputs(" ");
  LCDgotoxy(4, 0);
  LCD2n(dt);
  LCDgotoxy(6, 0);
  LCDputs("/");
  LCDgotoxy(7, 0);
  LCD2n(mth);
  LCDgotoxy(9, 0);
  LCDputs("/20");
  LCDgotoxy(12, 0);
  LCD2n(y);
  LCDgotoxy(14, 0);
  LCDputs("   ");

  LCDgotoxy(0, 1);
  LCDputs("Time: ");
  LCDgotoxy(6, 1);
  LCD2n(h24);
  LCDgotoxy(8, 1);
  LCDputs(":");
  LCDgotoxy(9, 1);
  LCD2n(min);
  LCDgotoxy(11, 1);
  LCDputs(":");
  LCDgotoxy(12, 1);
  LCD2n(sec);
  LCDgotoxy(14, 1);
  LCDputs("   ");
}
void read_DS()
{
  sec = DS_r(SEC);
  min = DS_r(MIN);
  h24 = DS_r(HR);
  day = DS_r(DAY);
  dt = DS_r(DT);
  mth = DS_r(MTH);
  y = DS_r(YEAR);
}
void cai_dat_thoi_gian()
{
  LCDgotoxy(0, 0);
  LCDputs("    CAI DAT     ");
  LCDgotoxy(0, 1);
  LCDputs("   THOI GIAN    ");
  delay(1500);
  LCDclear();
  show_DS();
  char flag = 1;
  char _cond = 0;
  char _ud = 2;

  while (flag == 1)
  {
    if (irrecv.decode(&results)) // náº¿u nháº­n Ä‘Æ°á»£c tÃ­n hiá»‡u
    {
      irrecv.resume();
      switch (results.value) // nhan phim
      {
      case 0xFFE21D: // phim tang
      {
        _ud = 3;
        break;
      }
      case 0xFFD22D: // phim giam
      {
        _ud = 1;
        break;
      }
      case 0xFF12ED: // phim qua trai
      {
        if (_cond >= 0)
          _cond--;
        if (_cond < 0)
          _cond = 6;
        break;
      }
      case 0xFF52AD: // phim qua phai
      {
        if (_cond <= 6)
          _cond++;
        if (_cond > 6)
          _cond = 0;
        break;
      }
      case 0xFF926D: // phim OK
      {
        flag = 0;
        _ud = 4;
        _cond = 7;
        DS_W(TimeStore::SEC, sec);  // sec
        DS_W(TimeStore::MIN, min);  // min
        DS_W(TimeStore::HOUR, h24); // h24
        DS_W(TimeStore::DAY, day);  // dat
        DS_W(TimeStore::DATE, dt);  // date
        DS_W(TimeStore::MTH, mth);  // moth
        DS_W(TimeStore::YEAR, y);   // year
        if (sec <= 12)
          preSec = sec + 11;
        else
          preSec = sec;
        preMin = min;
        preH24 = h24;
        tamp_day = 0;
        DS_W(Pre_Sec, preSec);
        DS_W(Pre_Min, preMin);
        DS_W(Pre_H24, preH24);
        DS_W(Tampday, tamp_day);
        LCDclear();
        delay(200);
        LCDgotoxy(0, 0);
        LCDputs("    CAI DAT     ");
        LCDgotoxy(0, 1);
        LCDputs("   HOAN  TAT   ");
        delay(1000);
        break;
      }
      default:
        break;
      }
      delay(200);
    }
    switch (_cond)
    {
    case 0:
    {
      switch (_ud)
      {
      case 3:
      {
        if (day <= 7)
          day++;
        if (day > 7)
          day = 1;
        LCDgotoxy(0, 0);
        LCDputs(day[thu]);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(0, 0);
        LCDputs(day[thu]);
        break;
      }
      case 1:
      {
        if (day >= 1)
          day--;
        if (day < 1)
          day = 7;
        LCDgotoxy(0, 0);
        LCDputs(day[thu]);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    case 1:
    {
      switch (_ud)
      {
      case 3:
      {
        if (dt <= 31)
          dt++;
        if (dt > 31)
          dt = 1;
        LCDgotoxy(4, 0);
        LCD2n(dt);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(4, 0);
        LCD2n(dt);
        break;
      }
      case 1:
      {
        if (dt >= 1)
          dt--;
        if (dt < 1)
          dt = 31;
        LCDgotoxy(4, 0);
        LCD2n(dt);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    case 2:
    {
      switch (_ud)
      {
      case 3:
      {
        if (mth <= 12)
          mth++;
        if (mth > 12)
          mth = 1;
        LCDgotoxy(7, 0);
        LCD2n(mth);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(7, 0);
        LCD2n(mth);
        break;
      }
      case 1:
      {
        if (mth >= 1)
          mth--;
        if (mth < 1)
          mth = 12;
        LCDgotoxy(7, 0);
        LCD2n(mth);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    case 3:
    {
      switch (_ud)
      {
      case 3:
      {
        if (y <= 99)
          y++;
        if (y > 99)
          y = 0;
        LCDgotoxy(12, 0);
        LCD2n(y);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(12, 0);
        LCD2n(y);
        break;
      }
      case 1:
      {
        if (y >= 0)
          y--;
        if (y < 0)
          y = 99;
        LCDgotoxy(12, 0);
        LCD2n(y);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    case 4:
    {
      switch (_ud)
      {
      case 3:
      {
        if (h24 <= 23)
          h24++;
        if (h24 > 23)
          h24 = 0;
        LCDgotoxy(6, 1);
        LCD2n(h24);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(6, 1);
        LCD2n(h24);
        break;
      }
      case 1:
      {
        if (h24 >= 0)
          h24--;
        if (h24 < 0)
          h24 = 23;
        LCDgotoxy(6, 1);
        LCD2n(h24);
        _ud = 2;
        break;
      }
      default:
        break;
      }
    }
    case 5:
    {
      switch (_ud)
      {
      case 3:
      {
        if (min <= 59)
          min++;
        if (min > 59)
          min = 0;
        LCDgotoxy(9, 1);
        LCD2n(min);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(9, 1);
        LCD2n(min);
        break;
      }
      case 1:
      {
        if (min >= 0)
          min--;
        if (min < 0)
          min = 59;
        LCDgotoxy(9, 1);
        LCD2n(min);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    case 6:
    {
      switch (_ud)
      {
      case 3:
      {
        if (sec <= 59)
          sec++;
        if (sec > 59)
          sec = 0;
        LCDgotoxy(12, 1);
        LCD2n(sec);
        _ud = 2;
        break;
      }
      case 2:
      {
        LCDgotoxy(12, 1);
        LCD2n(sec);
        break;
      }
      case 1:
      {
        if (sec >= 0)
          sec--;
        if (sec < 0)
          sec = 59;
        LCDgotoxy(12, 1);
        LCD2n(sec);
        _ud = 2;
        break;
      }
      default:
        break;
      }
      break;
    }
    default:
      break;
    }
  }
}
void chon_den_hen_gio()
{
  LCDclear();

  char flag = 1;
  char _cond = 1;
  char tamp_selt = 9;
  while (_cond != 0)
  {
    while (flag == 1)
    {
      LCDgotoxy(0, 0);
      LCDputs("CHON CHE DO: ");
      LCDgotoxy(13, 0);
      LCD1n(_cond);
      LCDgotoxy(14, 0);
      LCDputs("  ");
      LCDgotoxy(0, 1);
      LCDputs("CHON 0 DE THOAT ");

      if (irrecv.decode(&results))
      {
        irrecv.resume();
        switch (results.value) // nhan phim
        {
        case 0xFF926D: // phim ok
        {
          if (_cond != 0)
            flag = 2;
          else
            flag = 0;
          tamp_selt = 9;
          break;
        }
        case 0xFFE21D: // phim tang
        {
          if (_cond <= 4)
            _cond++;
          if (_cond > 4)
            _cond = 0;
          LCDgotoxy(13, 0);
          LCD1n(_cond);
          break;
        }
        case 0xFFD22D: // phim giam
        {
          if (_cond >= 0)
            _cond--;
          if (_cond < 0)
            _cond = 4;
          LCDgotoxy(13, 0);
          LCD1n(_cond);
          break;
        }
        default:
          break;
        }
      }
    }
    while (flag == 2)
    {
      if (irrecv.decode(&results))
      {
        irrecv.resume();
        switch (results.value) // nhan phim
        {
        case 0xFFE21D: // phim tang
        {
          _ud = 3;
          break;
        }
        case 0xFFD22D: // phim giam
        {
          _ud = 1;
          break;
        }
        case 0xFF12ED: // qua tria
        {
          if (_selt >= 0)
            _selt--;
          if (_selt < 0)
            _selt = 7;
          break;
        }
        case 0xFF52AD: // qua phai
        {
          if (_selt <= 7)
            _selt++;
          if (_selt > 7)
            _selt = 0;
          break;
        }
        case 0xFF926D: // phim OK
        {
          flag = 1;
          break;
        }
        default:
          break;
        }
        delay(50);
      }
      switch (_cond)
      {
      case 1:
      {
        if (_ud != 2 || _selt != tamp_selt)
        {
          set_time(S11, S12, PH11, PH12, GIO11, GIO12, SW11, SW12, 1);
          tamp_selt = _selt;
        }
        break;
      }
      case 2:
      {
        if (_ud != 2 || _selt != tamp_selt)
        {
          set_time(S21, S22, PH21, PH22, GIO21, GIO22, SW21, SW22, 2);
          tamp_selt = _selt;
        }
        break;
      }
      case 3:
      {
        if (_ud != 2 || _selt != tamp_selt)
        {
          set_time(S31, S32, PH31, PH32, GIO31, GIO32, SW31, SW32, 3);
          tamp_selt = _selt;
        }
        break;
      }
      case 4:
      {
        if (_ud != 2 || _selt != tamp_selt)
        {
          set_time(S41, S42, PH41, PH42, GIO41, GIO42, SW41, SW42, 4);
          tamp_selt = _selt;
        }
        break;
      }
      default:
        break;
      }
    }
  }
}
void set_time(unsigned char S1, unsigned char S2, unsigned char PH1, unsigned char PH2, unsigned char GIO1, unsigned char GIO2, unsigned char SW1, unsigned char SW2, char n)
{
  char giay1 = DS_r(S1);
  char phut1 = DS_r(PH1);
  char gio1 = DS_r(GIO1);
  char dk1 = DS_r(SW1);

  char giay2 = DS_r(S2);
  char phut2 = DS_r(PH2);
  char gio2 = DS_r(GIO2);
  char dk2 = DS_r(SW2);

  switch (_selt)
  {
  case 0:
  {
    switch (_ud)
    {
    case 3:
    {
      dk1 = 1;
      DS_W(SW1, dk1);
      _ud = 2;
      break;
    }
    case 1:
    {
      dk1 = 0;
      DS_W(SW1, dk1);
      _ud = 2;
      break;
    }
    default:
      break;
    }
    break;
  }
  case 1:
  {
    switch (_ud)
    {
    case 3:
    {
      if (gio1 <= 23)
        gio1++;
      if (gio1 > 23)
        gio1 = 0;
      DS_W(GIO1, gio1);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (gio1 >= 0)
        gio1--;
      if (gio1 < 0)
        gio1 = 23;
      DS_W(GIO1, gio1);
      _ud = 2;
      break;
    }
    default:
      break;
    }
  }
  case 2:
  {
    switch (_ud)
    {
    case 3:
    {
      if (phut1 <= 59)
        phut1++;
      if (phut1 > 59)
        phut1 = 0;
      DS_W(PH1, phut1);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (phut1 >= 0)
        phut1--;
      if (phut1 < 0)
        phut1 = 59;
      DS_W(PH1, phut1);
      _ud = 2;
      break;
    }
    default:
      break;
    }
  }
  case 3:
  {
    switch (_ud)
    {
    case 3:
    {
      if (giay1 <= 59)
        giay1++;
      if (giay1 > 59)
        giay1 = 0;
      DS_W(S1, giay1);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (giay1 >= 0)
        giay1--;
      if (giay1 < 0)
        giay1 = 59;
      DS_W(S1, giay1);
      _ud = 2;
      break;
    }
    default:
      break;
    }
  }
  case 4:
  {
    switch (_ud)
    {
    case 3:
    {
      dk2 = 1;
      DS_W(SW2, dk2);
      _ud = 2;
      break;
    }
    case 1:
    {
      dk2 = 0;
      DS_W(SW2, dk2);
      _ud = 2;
      break;
    }
    default:
      break;
    }
    break;
  }
  case 5:
  {
    switch (_ud)
    {
    case 3:
    {
      if (gio2 <= 23)
        gio2++;
      if (gio2 > 23)
        gio2 = 0;
      DS_W(GIO2, gio2);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (gio2 >= 0)
        gio2--;
      if (gio2 < 0)
        gio2 = 23;
      DS_W(GIO2, gio2);
      _ud = 2;
      break;
    }
    default:
      break;
    }
    break;
  }
  case 6:
  {
    switch (_ud)
    {
    case 3:
    {
      if (phut2 <= 59)
        phut2++;
      if (phut2 > 59)
        phut2 = 0;
      DS_W(PH2, phut2);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (phut2 >= 0)
        phut2--;
      if (phut2 < 0)
        phut2 = 59;
      DS_W(PH2, phut2);
      _ud = 2;
      break;
    }
    default:
      break;
    }
    break;
  }
  case 7:
  {
    switch (_ud)
    {
    case 3:
    {
      if (giay2 <= 59)
        giay2++;
      if (giay2 > 59)
        giay2 = 0;
      DS_W(S2, giay2);
      _ud = 2;
      break;
    }
    case 1:
    {
      if (giay2 >= 0)
        giay2--;
      if (giay2 < 0)
        giay2 = 59;
      DS_W(S2, giay2);
      _ud = 2;
      break;
    }
    default:
      break;
    }
    break;
  }
  default:
    break;
  }
  giay1 = DS_r(S1);
  phut1 = DS_r(PH1);
  gio1 = DS_r(GIO1);
  dk1 = DS_r(SW1);

  giay2 = DS_r(S2);
  phut2 = DS_r(PH2);
  gio2 = DS_r(GIO2);
  dk2 = DS_r(SW2);

  LCDgotoxy(0, 0);
  LCDputs("TGM");
  LCDgotoxy(3, 0);
  LCD1n(n);
  if (dk1 == 1)
  {
    LCDgotoxy(4, 0);
    LCDputs(" MO ");
  }
  else
  {
    LCDgotoxy(4, 0);
    LCDputs(" TAT");
  }
  LCDgotoxy(8, 0);
  LCD2n(gio1);
  LCDgotoxy(10, 0);
  LCDputs(":");
  LCDgotoxy(11, 0);
  LCD2n(phut1);
  LCDgotoxy(13, 0);
  LCDputs(":");
  LCDgotoxy(14, 0);
  LCD2n(giay1);

  LCDgotoxy(0, 1);
  LCDputs("TGT");
  LCDgotoxy(3, 1);
  LCD1n(n);
  if (dk2 == 1)
  {
    LCDgotoxy(4, 1);
    LCDputs(" MO ");
  }
  else
  {
    LCDgotoxy(4, 1);
    LCDputs(" TAT");
  }
  LCDgotoxy(8, 1);
  LCD2n(gio2);
  LCDgotoxy(10, 1);
  LCDputs(":");
  LCDgotoxy(11, 1);
  LCD2n(phut2);
  LCDgotoxy(13, 1);
  LCDputs(":");
  LCDgotoxy(14, 1);
  LCD2n(giay2);
}
void onoff()
{
  //***********Mo den 1*******************
  char s = DS_r(S11);
  char ph = DS_r(PH11);
  char gio = DS_r(GIO11);
  char sw = DS_r(SW11);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN1, 0);
  }
  //**********Tat den 1*****************
  s = DS_r(S12);
  ph = DS_r(PH12);
  gio = DS_r(GIO12);
  sw = DS_r(SW12);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN1, 1);
  }
  //***********Mo den 2*******************
  s = DS_r(S21);
  ph = DS_r(PH21);
  gio = DS_r(GIO21);
  sw = DS_r(SW21);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN2, 0);
  }
  //**********Tat den 2*****************
  s = DS_r(S22);
  ph = DS_r(PH22);
  gio = DS_r(GIO22);
  sw = DS_r(SW22);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN2, 1);
  }

  //***********Mo den 3*******************
  s = DS_r(S31);
  ph = DS_r(PH31);
  gio = DS_r(GIO31);
  sw = DS_r(SW31);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN3, 0);
  }
  //**********Tat den 3*****************
  s = DS_r(S32);
  ph = DS_r(PH32);
  gio = DS_r(GIO32);
  sw = DS_r(SW32);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN3, 1);
  }
  //***********Mo den 4*******************
  s = DS_r(S41);
  ph = DS_r(PH41);
  gio = DS_r(GIO41);
  sw = DS_r(SW41);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN4, 0);
  }
  //**********Tat den 4*****************
  s = DS_r(S42);
  ph = DS_r(PH42);
  gio = DS_r(GIO42);
  sw = DS_r(SW42);
  if (sw == 1)
  {
    if ((gio == h24) && (ph == min) && (s == sec))
      digitalWrite(DEN4, 1);
  }
}
