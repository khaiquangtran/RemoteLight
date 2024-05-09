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
  digitalWrite(light.LightLCD, LOW);
  lcd74595.LCDclear();
  pinMode(light.ButtonLight1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN11), onoff1, FALLING);
  pinMode(light.ButtonLight2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN21), onoff2, FALLING);
  pinMode(light.ButtonLight3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DEN31), onoff3, FALLING);

  preSec = ds1307.DS_r(Pre_Sec);
  preMin = ds1307.DS_r(Pre_Min);
  preH24 = ds1307.DS_r(Pre_H24);
  tamp_day = ds1307.DS_r(Tampday);
  previousDay = ds1307.DS_r(DAY);

  previousSec = ds1307.DS_r(SEC);
  tamp_sec = 0;
  flagDislayLCD = 1;

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
  if (flagDislayLCD == 1)
    show_DS();
  mode_on_off();
  mode_time_adjustment();
  mode_clear_lcd_display();
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
      mode_set_timer_light();
      break;
    }
    case (unsigned long)ButtonCode::Button1:
    {
      state1 = !state1;
      digitalWrite(light.Light1, state1);
      break;
    }
    case (unsigned long)ButtonCode::Button2:
    {
      state2 = !state2;
      digitalWrite(light.Light2, state2);
      break;
    }
    case (unsigned long)ButtonCode::Button3:
    {
      state3 = !state3;
      digitalWrite(light.Light3, state3);
      break;
    }
    case (unsigned long)ButtonCode::Button4:
    {
      state4 = !state4;
      digitalWrite(light.Light4, state4);
      break;
    }
    case (unsigned long)ButtonCode::Button5:
    {
      state5 = !state5;
      digitalWrite(light.LightLCD, state5);
      break;
    }
    case (unsigned long)ButtonCode::Button6:
    {
      tamp_sec = 0;
      flagDislayLCD = 1;
      break;
    }
    default:
      break;
    }
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
        lcd74595.LCDclear();
        flagDislayLCD = 1;
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
        // lcd74595.LCDgotoxy(0, 0);
        // lcd74595.LCDputs(day[thu]);
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
        // lcd74595.LCDgotoxy(4, 0);
        // lcd74595.LCD2n(dt);
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
        // lcd74595.LCDgotoxy(7, 0);
        // lcd74595.LCD2n(mth);
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
        // lcd74595.LCDgotoxy(12, 0);
        // lcd74595.LCD2n(y);
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
        // lcd74595.LCDgotoxy(6, 1);
        // lcd74595.LCD2n(h24);
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
        // lcd74595.LCDgotoxy(9, 1);
        // lcd74595.LCD2n(min);
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
        // lcd74595.LCDgotoxy(12, 1);
        // lcd74595.LCD2n(sec);
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

void app::set_time(unsigned char inSecond1, unsigned char inSecond2, unsigned char inMinute1, unsigned char inMinute2, unsigned char inHour1, unsigned char inHour2, unsigned char inSwitch1, unsigned char inSwitch2, OrderLight flagChooseLight, ListSetupTimer &flagMoveLeftRight, UpDown &flagUpDown)
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
  flagMoveLeftRight = ListSetupTimer::none;
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

void app::mode_set_timer_light()
{
  lcd74595.LCDclear();
  byte flagLoop = 1;
  OrderLight flagChooseLight = OrderLight::None;
  ListSetupTimer flagChooseTypeTime = ListSetupTimer::none;
  UpDown flagUpDown = UpDown::None;
  while (flagChooseLight != 0)
  {
    while (flagLoop == 1)
    {
      lcd74595.LCDgotoxy(0, 0);
      lcd74595.LCDputs("CHON DEN CAI DAT:");
      // lcd74595.LCDgotoxy(13, 0);
      // lcd74595.LCD1n(_cond);
      // lcd74595.LCDgotoxy(14, 0);
      // lcd74595.LCDputs("  ");
      lcd74595.LCDgotoxy(0, 1);
      lcd74595.LCDputs("  1   2   3   4  ");

      if (irrecv.decode(&results))
      {
        irrecv.resume();
        switch (results.value) // press button
        {
        case (unsigned long)ButtonCode::Button1:
        {
          flagChooseLight = OrderLight::Light1;
          flagLoop = 2;
          break;
        }
        case (unsigned long)ButtonCode::Button2:
        {
          flagChooseLight = OrderLight::Light2;
          flagLoop = 2;
          break;
        }
        case (unsigned long)ButtonCode::Button3:
        {
          flagChooseLight = OrderLight::Light3;
          flagLoop = 2;
          break;
        }
        case (unsigned long)ButtonCode::Button4:
        {
          flagChooseLight = OrderLight::Light4;
          flagLoop = 2;
          break;
        }
        default:
          break;
        }
      }
    }
    while (flagLoop == 2)
    {
      if (irrecv.decode(&results))
      {
        irrecv.resume();
        switch (results.value)
        {
        case (unsigned long)ButtonCode::ButtonUp:
        {
          flagUpDown = UpDown::Up;
          break;
        }
        case (unsigned long)ButtonCode::ButtonDown:
        {
          flagUpDown = UpDown::Down;
          break;
        }
        case (unsigned long)ButtonCode::ButtonLeft:
        {
          if (flagChooseTypeTime >= ListSetupTimer::switch1)
            flagChooseTypeTime--;
          if (flagChooseTypeTime < ListSetupTimer::switch1)
            flagChooseTypeTime = ListSetupTimer::second2;
          break;
        }
        case (unsigned long)ButtonCode::ButtonRight:
        {
          if (flagChooseTypeTime <= ListSetupTimer::second2)
            flagChooseTypeTime++;
          if (flagChooseTypeTime > ListSetupTimer::second2)
            flagChooseTypeTime = ListSetupTimer::switch1;
          break;
        }
        case (unsigned long)ButtonCode::ButtonOk:
        {
          flagLoop = 1;
          lcd74595.LCDclear();
          break;
        }
        default:
          break;
        }
        delay(50);
      }
      switch (flagChooseLight)
      {
      case OrderLight::Light1:
      {
        if (flagUpDown != UpDown::None || flagChooseTypeTime != ListSetupTimer::none;)
        {
          set_time(SecondFisrt_1, SecondFisrt_2, MinuteFisrt_1, MinuteFisrt_2, HourFisrt_1, HourFisrt_2, SwitchFirst_1, SwitchFirst_2, OrderLight::Light1, flagChooseTypeTime, flagUpDown);
        }
        break;
      }
      case OrderLight::Light2:
      {
        if (flagUpDown != UpDown::None || flagChooseTypeTime != ListSetupTimer::none;)
        {
          set_time(SecondSecond_1, SecondSecond_2, MinuteSecond_1, MinuteSecond_2, HourSecond_1, HourSecond_2, SwitchSecond_1, SwitchSecond_2, OrderLight::Light2, flagChooseTypeTime, flagUpDown);
        }
        break;
      }
      case OrderLight::Light3:
      {
        if (flagUpDown != UpDown::None || flagChooseTypeTime != ListSetupTimer::none;)
        {
          set_time(SecondThird_1, SecondThird_2, MinuteThird_1, MinuteThird_2, HourThird_1, HourThird_2, SwitchThird_1, SwitchThird_2, OrderLight::Light3, flagChooseTypeTime, flagUpDown);
        }
        break;
      }
      case OrderLight::Light4:
      {
        if (flagUpDown != UpDown::None || flagChooseTypeTime != ListSetupTimer::none;)
        {
          set_time(SecondFourth_1, SecondFourth_2, MinuteFourth_1, MinuteFourth_2, HourFourth_1, HourFourth_2, SwitchFourth_1, SwitchFourth_2, OrderLight::Light4, flagChooseTypeTime, flagUpDown);
        }
        break;
      }
      default:
        break;
      }
    }
  }
}

void app::mode_time_adjustment()
{
  if ((previousDay > day || previousDay < day) && tamp_day < NumberDayAdjustment)
  {
    tamp_day++;
    ds1307.DS_W(Tampday, tamp_day);
    previousDay = day;
  }
  if (tamp_day == 4 && preSec == sec && preMin == min && preH24 == h24)
  {
    sec = sec - 11; // tolerance time
    ds1307.DS_W(SEC, sec);
    tamp_day = 0;
    ds1307.DS_W(Tampday, tamp_day);
  }
}

void app::mode_clear_lcd_display()
{
  if ((previousSec > sec || previousSec < sec) && tamp_sec < 30)
  {
    tamp_sec++;
    previousSec = sec;
  }
  if (tamp_sec == 30)
  {
    flagDislayLCD = 0;
    tamp_sec = 31;
    lcd74595.LCDclear();
  }
}


void app::mode_on_off()
{
  
  byte second = ds1307.DS_r(SecondFisrt_1);
  byte minute = ds1307.DS_r(MinuteFisrt_1);
  byte hour = ds1307.DS_r(HourFisrt_1);
  byte sw = ds1307.DS_r(SwitchFirst_1);
  //***********On light 1*******************
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light1, LOW);
  }
  //**********Off light 1*****************
  second = ds1307.DS_r(SecondFisrt_2);
  minute = ds1307.DS_r(MinuteFisrt_2);
  hour = ds1307.DS_r(HourFisrt_2);
  sw = ds1307.DS_r(SwitchFirst_2);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light1, HIGH);
  }
  //***********On light 2*******************
  second = ds1307.DS_r(SecondSecond_1);
  minute = ds1307.DS_r(MinuteSecond_1);
  hour = ds1307.DS_r(HourSecond_1);
  sw = ds1307.DS_r(SwitchSecond_1);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light2, LOW);
  }
  //**********Off light 2*****************
  second = ds1307.DS_r(SecondSecond_2);
  minute = ds1307.DS_r(MinuteSecond_2);
  hour = ds1307.DS_r(HourSecond_2);
  sw = ds1307.DS_r(SwitchSecond_2);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light2, HIGH);
  }

  //***********On light 3*******************
  second = ds1307.DS_r(SecondThird_1);
  minute = ds1307.DS_r(MinuteThird_1);
  hour = ds1307.DS_r(HourThird_1);
  sw = ds1307.DS_r(SwitchThird_1);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light3, LOW);
  }
  //**********Off light 3*****************
  second = ds1307.DS_r(SecondThird_2);
  minute = ds1307.DS_r(MinuteThird_2);
  hour = ds1307.DS_r(HourThird_2);
  sw = ds1307.DS_r(SwitchThird_2);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light3, HIGH);
  }
  //***********On light 4*******************
  second = ds1307.DS_r(SecondFourth_1);
  minute = ds1307.DS_r(MinuteFourth_1);
  hour = ds1307.DS_r(HourFourth_1);
  sw = ds1307.DS_r(SwitchFourth_1);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light4, LOW);
  }
  //**********Off light 4*****************
  second = ds1307.DS_r(SecondFourth_2);
  minute = ds1307.DS_r(MinuteFourth_2);
  hour = ds1307.DS_r(HourFourth_2);
  sw = ds1307.DS_r(SwitchFourth_2);
  if (sw == ON)
  {
    if ((hour == h24) && (minute == min) && (second == sec))
      digitalWrite(light.Light4, HIGH);
  }
}
