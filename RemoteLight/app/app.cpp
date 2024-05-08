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
  pinMode(Light::ButtonLight1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN11), onoff1, FALLING);
  pinMode(Light::ButtonLight2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN21), onoff2, FALLING);
  pinMode(Light::ButtonLight3, INPUT_PULLUP);
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
  if (irrecv.decode(&results))
  {
    irrecv.resume();
    switch (results.value)
    {
    case (unsigned long)ButtonCode::ButtonApp:
    {
      setup_time();
      break;
    }
    case (unsigned long)ButtonCode::ButtonMenu:
    {
      chon_den_hen_gio();
      break;
    }
    case (unsigned long)ButtonCode::Button1:
    {
      state1 = !state1;
      digitalWrite(DEN1, state1);
      break;
    }
    case (unsigned long)ButtonCode::Button2:
    {
      state2 = !state2;
      digitalWrite(DEN2, state2);
      break;
    }
    case (unsigned long)ButtonCode::Button3:
    {
      state3 = !state3;
      digitalWrite(DEN3, state3);
      break;
    }
    case (unsigned long)ButtonCode::Button4:
    {
      state4 = !state4;
      digitalWrite(DEN4, state4);
      break;
    }
    case (unsigned long)ButtonCode::Button5:
    {
      state5 = !state5;
      digitalWrite(DENLCD, state5);
      break;
    }
    case (unsigned long)ButtonCode::Button6:
    {
      tamp_sec = 0;
      flag = 1;
      break;
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
    ds1307.LCDclear();
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
  lcd74595.LCDgotoxy(Column::Three, Row::One);
  lcd74595.LCDputs(" ");
  lcd74595.LCDgotoxy(Column::Four, Row::One);
  lcd74595.LCD2n(dt);
  lcd74595.LCDgotoxy(Column::Six, Row::One);
  lcd74595.LCDputs("/");
  lcd74595.LCDgotoxy(Column::Seven, Row::One);
  lcd74595.LCD2n(mth);
  lcd74595.LCDgotoxy(Column::Nine, Row::One);
  lcd74595.LCDputs("/20");
  lcd74595.LCDgotoxy(Column::Twelve, Row::One);
  lcd74595.LCD2n(y);
  lcd74595.LCDgotoxy(Column::Fourteen, Row::One);
  lcd74595.LCDputs("   ");

  lcd74595.LCDgotoxy(Column::Zero, Row::Tow);
  lcd74595.LCDputs("Time: ");
  lcd74595.LCDgotoxy(Column::Six, Row::Tow);
  lcd74595.LCD2n(h24);
  lcd74595.LCDgotoxy(Column::Eight, Row::Tow);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Nine, Row::Tow);
  lcd74595.LCD2n(min);
  lcd74595.LCDgotoxy(Column::Eleven, Row::Tow);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Twelve, Row::Tow);
  lcd74595.LCD2n(sec);
  lcd74595.LCDgotoxy(Column::Fourteen, Row::Tow);
  lcd74595.LCDputs("   ");
}

void app::setup_time()
{
  lcd74595.LCDgotoxy(Column::Zero, Row::Zero);
  lcd74595.LCDputs("    CAI DAT     ");
  lcd74595.LCDgotoxy(Column::Zero, Row::One);
  lcd74595.LCDputs("   THOI GIAN    ");
  delay(1500);
  lcd74595.LCDclear();
  show_DS();
  byte flagLoop = 1;
  ListSetupTime flagMoveLeftRight = ListSetupTime::day;
  UpDown flagUpDown = UpDown::None;
	
  while (flagLoop == 1)
  {
    if (irrecv.decode(&results)) 
    {
      irrecv.resume();
      switch (results.value) 
      {
      case ButtonCode::ButtonUp: 
      {
        flagUpDown = UpDown::Up;
        break;
      }
      case ButtonCode::ButtonDown:
      {
        flagUpDown = UpDown::Down;
        break;
      }
      case ButtonCode::ButtonLeft: 
      {
        if (flagMoveLeftRight >= ListSetupTime::day)
          flagMoveLeftRight--;
        if (flagMoveLeftRight < ListSetupTime::day)
          flagMoveLeftRight = ListSetupTime::second;
        break;
      }
      case ButtonCode::ButtonRight: 
      {
        if (flagMoveLeftRight <= ListSetupTime::second)
          flagMoveLeftRight++;
        if (flagMoveLeftRight > ListSetupTime::second)
          flagMoveLeftRight = ListSetupTime::day;
        break;
      }
      case ButtonCode::ButtonOk:
      {
        flagLoop = 0;
        flagUpDown = UpDown::Quit;
        flagMoveLeftRight = ListSetupTime::quit;
        ds1307.DS_W(TimeStore::SEC, sec);  // sec
        ds1307.DS_W(TimeStore::MIN, min);  // min
        ds1307.DS_W(TimeStore::HOUR, h24); // h24
        ds1307.DS_W(TimeStore::DAY, day);  // dat
        ds1307.DS_W(TimeStore::DATE, dt);  // date
        ds1307.DS_W(TimeStore::MTH, mth);  // moth
        ds1307.DS_W(TimeStore::YEAR, y);   // year
        if (sec <= 12)
          preSec = sec + 11;
        else
          preSec = sec;
        preMin = min;
        preH24 = h24;
        tamp_day = 0;
        ds1307.DS_W(Pre_Sec, preSec);
        ds1307.DS_W(Pre_Min, preMin);
        ds1307.DS_W(Pre_H24, preH24);
        ds1307.DS_W(Tampday, tamp_day);
        lcd74595.LCDclear();
        delay(200);
        lcd74595.LCDgotoxy(Column::Zero, Row::One);
        lcd74595.LCDputs("    CAI DAT     ");
        lcd74595.LCDgotoxy(Column::Zero, 1);
        lcd74595.LCDputs("   HOAN  TAT   ");
        delay(1000);
        break;
      }
      default:
        break;
      }
      delay(200);
    }
    switch (flagMoveLeftRight)
    {
    case (byte)ListSetupTime::day:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (day <= dayMax)
          day++;
        if (day > dayMax)
          day = dayMin;
        lcd74595.LCDgotoxy(Column::Zero, Row::One);
        lcd74595.LCDputs(day[thu]);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(0, 0);
        //lcd74595.LCDputs(day[thu]);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (day >= dayMin)
          day--;
        if (day < dayMin)
          day = dayMax;
        lcd74595.LCDgotoxy(Column::Zero, Row::One);
        lcd74595.LCDputs(day[thu]);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (byte)ListSetupTime::date:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (dt <= dateMax)
          dt++;
        if (dt > dateMax)
          dt = dateMin;
        lcd74595.LCDgotoxy(Column::Four, Row::One);
        lcd74595.LCD2n(dt);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(4, 0);
        //lcd74595.LCD2n(dt);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (dt >= dateMin)
          dt--;
        if (dt < dateMin)
          dt = dateMax;
        lcd74595.LCDgotoxy(Column::Four, Row::One);
        lcd74595.LCD2n(dt);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (byte)ListSetupTime::month:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (mth <= monthMax)
          mth++;
        if (mth > monthMax)
          mth = monthMin;
        lcd74595.LCDgotoxy(Column::Seven, Row::One);
        lcd74595.LCD2n(mth);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(7, 0);
        //lcd74595.LCD2n(mth);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (mth >= monthMin)
          mth--;
        if (mth < monthMin)
          mth = monthMax;
        lcd74595.LCDgotoxy(Column::Seven, Row::One);
        lcd74595.LCD2n(mth);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (byte)ListSetupTime::year:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (y <= yearMax)
          y++;
        if (y > yearMax)
          y = yearMin;
        lcd74595.LCDgotoxy(Column::Twelve, Row::One);
        lcd74595.LCD2n(y);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(12, 0);
        //lcd74595.LCD2n(y);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (y >= yearMin)
          y--;
        if (y < yearMin)
          y = yearMax;
        lcd74595.LCDgotoxy(Column::Twelve, Row::One);
        lcd74595.LCD2n(y);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (byte)ListSetupTime::hour:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (h24 <= hourMax)
          h24++;
        if (h24 > hourMax)
          h24 = hourMin;
        lcd74595.LCDgotoxy(Column::Six, Row::Two);
        lcd74595.LCD2n(h24);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(6, 1);
        //lcd74595.LCD2n(h24);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (h24 >= hourMin)
          h24--;
        if (h24 < hourMin)
          h24 = hourMax;
        lcd74595.LCDgotoxy(Column::Six, Row::Two);
        lcd74595.LCD2n(h24);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
    }
    case (byte)ListSetupTime::minute:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (min <= minuteMax)
          min++;
        if (min > minuteMax)
          min = minuteMin;
        lcd74595.LCDgotoxy(Column::Nine, Row::Two);
        lcd74595.LCD2n(min);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(9, 1);
        //lcd74595.LCD2n(min);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (min >= minuteMin)
          min--;
        if (min < minuteMin)
          min = minuteMax;
        lcd74595.LCDgotoxy(Column::Nine, Row::Two);
        lcd74595.LCD2n(min);
        flagUpDown = UpDown::None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (byte)ListSetupTime::second:
    {
      switch (flagUpDown)
      {
      case (byte)UpDown::Up:
      {
        if (sec <= secondMax)
          sec++;
        if (sec > secondMax)
          sec = secondMin;
        lcd74595.LCDgotoxy(Column::Twelve, Row::Two);
        lcd74595.LCD2n(sec);
        flagUpDown = UpDown::None;
        break;
      }
      case (byte)UpDown::None:
      {
        //lcd74595.LCDgotoxy(12, 1);
        //lcd74595.LCD2n(sec);
        break;
      }
      case (byte)UpDown::Down:
      {
        if (sec >= secondMin)
          sec--;
        if (sec < secondMin)
          sec = secondMax;
        lcd74595.LCDgotoxy(Column::Twelve, Row::Two);
        lcd74595.LCD2n(sec);
        flagUpDown = UpDown::None;
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

void app::set_time(unsigned char inSecond1, unsigned char inSecond2, unsigned char inMinute1, unsigned char inMinute2, unsigned char inHour1, unsigned char inHour2, unsigned char inSwitch1, unsigned char inSwitch2, unsigned char n, ListSetupTimer& flagMoveLeftRight, UpDown& flagUpDown)
{
  byte second1 = ds1307.DS_r(inSecond1);
  byte minute1 = ds1307.DS_r(inMinute1);
  byte hour1 = ds1307.DS_r(inHour1);
  byte switch1 = ds1307.DS_r(inSwitch1);

  byte second2 = ds1307.DS_r(inSecond2);
  byte minute2 = ds1307.DS_r(inMinute2);
  byte hour2 = ds1307.DS_r(inHour2);
  byte switch2 = ds1307.DS_r(inSwitch2);

  switch (flagMoveLeftRight)
  {
  case ListSetupTimer::switch1:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      switch1 = ON
      ds1307.DS_W(inSwitch1, switch1);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      switch1 = OFF;
      ds1307.DS_W(inSwitch1, switch1);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case ListSetupTimer::hour1:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (hour1 <= hourMax)
        hour1++;
      if (hour1 > hourMax)
        hour1 = hourMin;
      ds1307.DS_W(inHour1, hour1);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (hour1 >= hourMin)
        hour1--;
      if (hour1 < hourMin)
        hour1 = hourMax;
      ds1307.DS_W(inHour1, hour1);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
  }
  case ListSetupTimer::minute1:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (minute1 <= minuteMax)
        minute1++;
      if (minute1 > minuteMax)
        minute1 = minuteMin;
      ds1307.DS_W(inMinute1, minute1);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (minute1 >= minuteMin)
        minute1--;
      if (minute1 < minuteMin)
        minute1 = minuteMax;
      ds1307.DS_W(inMinute1, minute1);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
  }
  case ListSetupTimer::second1:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (second1 <= secondMax)
        second1++;
      if (second1 > secondMax)
        second1 = secondMin;
      ds1307.DS_W(inSecond1, second1);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (second1 >= secondMin)
        second1--;
      if (second1 < secondMin)
        second1 = secondMax;
      ds1307.DS_W(inSecond1, second1);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
  }
  case ListSetupTimer::switch2:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      switch2 = ON;
      ds1307.DS_W(inSwitch2, switch2);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      switch2 = OFF;
      ds1307.DS_W(inSwitch2, switch2);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case ListSetupTimer::hour2:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (hour2 <= hourMax)
        hour2++;
      if (hour2 > hourMax)
        hour2 = hourMin;
      ds1307.DS_W(inHour2, hour2);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (hour2 >= hourMin)
        hour2--;
      if (hour2 < hourMin)
        hour2 = hourMax;
      ds1307.DS_W(inHour2, hour2);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case ListSetupTimer::minute2:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (minute2 <= minuteMax)
        minute2++;
      if (minute2 > minuteMax)
        minute2 = minuteMin;
      ds1307.DS_W(inMinute2, minute2);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (minute2 >= minuteMin)
        minute2--;
      if (minute2 < minuteMin)
        minute2 = minuteMax;
      ds1307.DS_W(inMinute2, minute2);
      flagUpDown = UpDown::None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case ListSetupTimer::second2:
  {
    switch (flagUpDown)
    {
    case UpDown::Up:
    {
      if (second2 <= secondMax)
        second2++;
      if (second2 > secondMax)
        second2 = secondMin;
      ds1307.DS_W(inSecond2, second2);
      flagUpDown = UpDown::None;
      break;
    }
    case UpDown::Down:
    {
      if (second2 >= secondMin)
        second2--;
      if (second2 < secondMin)
        second2 = secondMax;
      ds1307.DS_W(inSecond2, second2);
      flagUpDown = UpDown::None;
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
  second1 = ds1307.DS_r(inSecond1);
  minute1 = ds1307.DS_r(inMinute1);
  hour1 = ds1307.DS_r(inHour1);
  switch1 = ds1307.DS_r(inSwitch1);

  second2 = ds1307.DS_r(inSecond2);
  minute2 = ds1307.DS_r(inMinute2);
  hour2 = ds1307.DS_r(inHour2);
  switch2 = ds1307.DS_r(inSwitch2);

  lcd74595.LCDgotoxy(Column::Zero, Row::One);
  lcd74595.LCDputs("TGM");
  lcd74595.LCDgotoxy(Column::Three, Row::One);
  lcd74595.LCD1n(n);
  if (switch1 == ON)
  {
    lcd74595.LCDgotoxy(Column::Four, Row::One);
    lcd74595.LCDputs(" MO ");
  }
  else
  {
    lcd74595.LCDgotoxy(Column::Four, Row::One);
    lcd74595.LCDputs(" TAT");
  }
  lcd74595.LCDgotoxy(Column::Eight, Row::One);
  lcd74595.LCD2n(hour1);
  lcd74595.LCDgotoxy(Column::Ten, Row::One);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Eleven, Row::One);
  lcd74595.LCD2n(minute1);
  lcd74595.LCDgotoxy(Column::Thirteen, Row::One);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Fourteen, Row::One);
  lcd74595.LCD2n(second1);

  lcd74595.LCDgotoxy(Column::Three, Row::Two);
  lcd74595.LCDputs("TGT");
  lcd74595.LCDgotoxy(Column::Three, Row::Two);
  lcd74595.LCD1n(n);
  if (switch2 == ON)
  {
    lcd74595.LCDgotoxy(Column::Four, Row::Two);
    lcd74595.LCDputs(" MO ");
  }
  else
  {
    lcd74595.LCDgotoxy(Column::Four, Row::Two);
    lcd74595.LCDputs(" TAT");
  }
  lcd74595.LCDgotoxy(Column::Eight, Row::Two);
  lcd74595.LCD2n(hour2);
  lcd74595.LCDgotoxy(Column::Ten, Row::Two);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Eleven, Row::Two);
  lcd74595.LCD2n(minute2);
  lcd74595.LCDgotoxy(Column::Thirteen, Row::Two);
  lcd74595.LCDputs(":");
  lcd74595.LCDgotoxy(Column::Fourteen, Row::Two);
  lcd74595.LCD2n(second2);
}
