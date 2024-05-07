#include "app.h"

void app::init()
{
    irrecv.enableIRIn();
    lcd74595.LCDinit();
    lcd74595.LCDgotoxy(0, 0);
    lcd74595.LCDputs("    WELCOME     ");
    lcd74595.LCDgotoxy(0, 1);
    lcd74595.LCDputs("KHAI'S EQUIPMENT");
    delay(1500);
    digitalWrite(DENLCD, LOW);
    lcd74595.LCDclear();
    pinMode(DEN11, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DEN11), onoff1, FALLING);
    pinMode(DEN21, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DEN21), onoff2, FALLING);
    pinMode(DEN31, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DEN31), onoff3, FALLING);

    preSec = ds1307.DS_r(Pre_Sec);
    preMin = ds1307.DS_r(Pre_Min);
    preH24 = ds1307.DS_r(Pre_H24);
    tamp_day = ds1307.DS_r(Tampday);
    previousDay = ds1307.DS_r(DAY);

    previousSec = ds1307.DS_r(SEC);
    tamp_sec = 0;
    flag = 1;

    /*ds1307.DS_W(0x00,0); //sec
    ds1307.DS_W(0x01,0);  //min
    ds1307.DS_W(0x02,4);  //h24
    ds1307.DS_W(0x03,7);  //dat
    ds1307.DS_W(0x04,23);  //date
    ds1307.DS_W(0x05,7); //moth
    ds1307.DS_W(0x06,22); //year*/
}

void app::config()
{
    pinMode(light.Light1, OUTPUT);
    pinMode(light.Light2, OUTPUT);
    pinMode(light.Light3, OUTPUT);
    pinMode(light.Light4, OUTPUT);
    pinMode(light.LightLCD, OUTPUT);

    digitalWrite(light.Light4, HIGH);
    digitalWrite(light.Light4, HIGH);
    digitalWrite(light.Light4, HIGH);
    digitalWrite(light.Light4, HIGH);
    digitalWrite(light.LightLCD, HIGH);
}

void app::run()
{
    read_DS();
    if (flag == 1)
        show_DS();
    onoff();
    if (irrecv.decode(&results)) // náº¿u nháº­n Ä‘Æ°á»£c tÃ­n hiá»‡u
    {
        irrecv.resume();       // nháº­n giÃ¡ trá»‹ tiáº¿p theo
        switch (results.value) // phim App
        {
        case (unsigned long)ButtonCode::ButtonApp:
        { // phim App
            setup_time();
            break;
        }
        case (unsigned long)ButtonCode::ButtonMenu:
        { // phim menu
            chon_den_hen_gio();
            break;
        }
        case (unsigned long)ButtonCode::Button1:
        { // phim 1
            state1 = !state1;
            digitalWrite(DEN1, state1);
            break;
        }
        case (unsigned long)ButtonCode::Button2:
        { // phim 2
            state2 = !state2;
            digitalWrite(DEN2, state2);
            break;
        }
        case (unsigned long)ButtonCode::Button3:
        { // phim 3
            state3 = !state3;
            digitalWrite(DEN3, state3);
            break;
        }
        case (unsigned long)ButtonCode::Button4:
        { // phim 4
            state4 = !state4;
            digitalWrite(DEN4, state4);
            break;
        }
        case (unsigned long)ButtonCode::Button5:
        { // phim 5
            state5 = !state5;
            digitalWrite(DENLCD, state5);
            break;
        }
        case (unsigned long)ButtonCode::Button6:
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
        sec = sec - 11; // tolerance time
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

void app::read_DS()
{
    sec = ds1307.DS_r(TimeStore::SEC);
    min = ds1307.DS_r(TimeStore::MIN);
    h24 = ds1307.DS_r(TimeStore::HOUR);
    day = ds1307.DS_r(TimeStore::DAY);
    dt = ds1307.DS_r(TimeStore::DATE);
    mth = ds1307.DS_r(TimeStore::MTH);
    y = ds1307.DS_r(TimeStore::YEAR);
}

void app::show_DS()
{
    lcd74595.LCDgotoxy(0, 0);
    lcd74595.LCDputs(day[thu]);
    lcd74595.LCDgotoxy(3, 0);
    lcd74595.LCDputs(" ");
    lcd74595.LCDgotoxy(4, 0);
    lcd74595.LCD2n(dt);
    lcd74595.LCDgotoxy(6, 0);
    lcd74595.LCDputs("/");
    lcd74595.LCDgotoxy(7, 0);
    lcd74595.LCD2n(mth);
    lcd74595.LCDgotoxy(9, 0);
    lcd74595.LCDputs("/20");
    lcd74595.LCDgotoxy(12, 0);
    lcd74595.LCD2n(y);
    lcd74595.LCDgotoxy(14, 0);
    lcd74595.LCDputs("   ");

    lcd74595.LCDgotoxy(0, 1);
    lcd74595.LCDputs("Time: ");
    lcd74595.LCDgotoxy(6, 1);
    lcd74595.LCD2n(h24);
    lcd74595.LCDgotoxy(8, 1);
    lcd74595.LCDputs(":");
    lcd74595.LCDgotoxy(9, 1);
    lcd74595.LCD2n(min);
    lcd74595.LCDgotoxy(11, 1);
    lcd74595.LCDputs(":");
    lcd74595.LCDgotoxy(12, 1);
    lcd74595.LCD2n(sec);
    lcd74595.LCDgotoxy(14, 1);
    lcd74595.LCDputs("   ");
}

void app::setup_time()
{
    lcd74595.LCDgotoxy(0, 0);
    lcd74595.LCDputs("    CAI DAT     ");
    lcd74595.LCDgotoxy(0, 1);
    lcd74595.LCDputs("   THOI GIAN    ");
    lcd74595.delay(1500);
    lcd74595.LCDclear();
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
                lcd74595.LCDclear();
                delay(200);
                lcd74595.LCDgotoxy(0, 0);
                lcd74595.LCDputs("    CAI DAT     ");
                LCDgotoxy(0, 1);
                lcd74595.LCDputs("   HOAN  TAT   ");
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
                lcd74595.LCDgotoxy(0, 0);
                lcd74595.LCDputs(day[thu]);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(0, 0);
                lcd74595.LCDputs(day[thu]);
                break;
            }
            case 1:
            {
                if (day >= 1)
                    day--;
                if (day < 1)
                    day = 7;
                lcd74595.LCDgotoxy(0, 0);
                lcd74595.LCDputs(day[thu]);
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
                lcd74595.LCDgotoxy(4, 0);
                lcd74595.LCD2n(dt);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(4, 0);
                lcd74595.LCD2n(dt);
                break;
            }
            case 1:
            {
                if (dt >= 1)
                    dt--;
                if (dt < 1)
                    dt = 31;
                lcd74595.LCDgotoxy(4, 0);
                lcd74595.LCD2n(dt);
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
                lcd74595.LCDgotoxy(7, 0);
                lcd74595.LCD2n(mth);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(7, 0);
                lcd74595.LCD2n(mth);
                break;
            }
            case 1:
            {
                if (mth >= 1)
                    mth--;
                if (mth < 1)
                    mth = 12;
                lcd74595.LCDgotoxy(7, 0);
                lcd74595.LCD2n(mth);
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
                lcd74595.LCDgotoxy(12, 0);
                lcd74595.LCD2n(y);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(12, 0);
                lcd74595.LCD2n(y);
                break;
            }
            case 1:
            {
                if (y >= 0)
                    y--;
                if (y < 0)
                    y = 99;
                lcd74595.LCDgotoxy(12, 0);
                lcd74595.LCD2n(y);
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
                lcd74595.LCDgotoxy(6, 1);
                lcd74595.LCD2n(h24);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(6, 1);
                lcd74595.LCD2n(h24);
                break;
            }
            case 1:
            {
                if (h24 >= 0)
                    h24--;
                if (h24 < 0)
                    h24 = 23;
                lcd74595.LCDgotoxy(6, 1);
                lcd74595.LCD2n(h24);
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
                lcd74595.LCDgotoxy(9, 1);
                lcd74595.LCD2n(min);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(9, 1);
                lcd74595.LCD2n(min);
                break;
            }
            case 1:
            {
                if (min >= 0)
                    min--;
                if (min < 0)
                    min = 59;
                lcd74595.LCDgotoxy(9, 1);
                lcd74595.LCD2n(min);
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
                lcd74595.LCDgotoxy(12, 1);
                lcd74595.LCD2n(sec);
                _ud = 2;
                break;
            }
            case 2:
            {
                lcd74595.LCDgotoxy(12, 1);
                lcd74595.LCD2n(sec);
                break;
            }
            case 1:
            {
                if (sec >= 0)
                    sec--;
                if (sec < 0)
                    sec = 59;
                lcd74595.LCDgotoxy(12, 1);
                lcd74595.LCD2n(sec);
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



void app::set_time(unsigned char S1, unsigned char S2, unsigned char PH1, unsigned char PH2, unsigned char GIO1, unsigned char GIO2, unsigned char SW1, unsigned char SW2, char n)
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