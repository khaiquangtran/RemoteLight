#include "app.h"

app::app()
{
  lcd74595 = new LCD74595(P1_0, P1_1, P1_2);
  ds1307 = new DS1307(P1_6, P1_7);
  irrecv = new IRrecv(P1_5);
}

app::~app()
{
  delete lcd74595;
  delete ds1307;
  delete irrecv;
}

void app::init()
{
  config();
  irrecv->enableIRIn();
  lcd74595->LCDinit();
  print_hello();
  init_interrupt();
  read_time();
  preSec = ds1307->DS_r(PreSecond);
  variabilitySecond = preSec;
  preMin = ds1307->DS_r(PreMinute);
  variabilityMinute = preMin;
  preH24 = ds1307->DS_r(PreHour);
  variabilityHour = preH24;
  tamp_day = ds1307->DS_r(Tampday);
  previousDay = ds1307->DS_r(DAY);

  previousSec = ds1307->DS_r(SEC);
  tamp_sec = 0;
  flagDislayLCD = 1;

  read_time_installed();
}

void app::run()
{
  read_DS();
  mode_time_adjustment();
  mode_clear_lcd_display();
  mode_receive_IR();
  delay(50);
}

void app::config()
{
  pinMode(Light1, OUTPUT);
  pinMode(Light2, OUTPUT);
  pinMode(Light3, OUTPUT);
  pinMode(Light4, OUTPUT);
  pinMode(LightLCD, OUTPUT);

  digitalWrite(Light1, HIGH);
  digitalWrite(Light2, HIGH);
  digitalWrite(Light3, HIGH);
  digitalWrite(Light4, HIGH);
  digitalWrite(LightLCD, HIGH);
}

bool app::stateLight1 = ON;
bool app::stateLight2 = ON;
bool app::stateLight3 = ON;
bool app::stateLight4 = ON;
bool app::stateLightLCD = ON;

void app::onOffLight1()
{
  detachInterrupt(digitalPinToInterrupt(ButtonLight1));
  stateLight1 = !stateLight1;
  digitalWrite(Light1, stateLight1);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(ButtonLight1), onOffLight1, FALLING);
}

void app::onOffLight2()
{
  detachInterrupt(digitalPinToInterrupt(ButtonLight2));
  stateLight2 = !stateLight2;
  digitalWrite(Light2, stateLight2);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(ButtonLight2), onOffLight2, FALLING);
}

void app::onOffLight3()
{
  detachInterrupt(digitalPinToInterrupt(ButtonLight3));
  stateLight3 = !stateLight3;
  digitalWrite(Light3, stateLight3);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(ButtonLight3), onOffLight3, FALLING);
}

void app::read_DS()
{
  sec = ds1307->DS_r(SEC);
  if (sec != variabilitySecond)
  {
    min = ds1307->DS_r(MIN);
    mode_on_off();
    if (flagDislayLCD == 1)
      show_DS();
    variabilitySecond = sec;
    if (min != variabilityMinute)
    {
      h24 = ds1307->DS_r(HOUR);
      variabilityMinute = min;
      if (h24 != variabilityHour)
      {
        day = ds1307->DS_r(DAY);
        dt = ds1307->DS_r(DATE);
        mth = ds1307->DS_r(MTH);
        y = ds1307->DS_r(YEAR);
        variabilityHour = h24;
      }
    }
  }
}

void app::show_DS()
{
  lcd74595->LCDgotoxy(Zero, RowOne);
  lcd74595->LCDputs(day[thu]);
  // lcd74595->LCDgotoxy(Three, RowOne);
  lcd74595->LCDputs(" ");
  // lcd74595->LCDgotoxy(Four, RowOne);
  lcd74595->LCD2n(dt);
  // lcd74595->LCDgotoxy(Six, RowOne);
  lcd74595->LCDputs("/");
  // lcd74595->LCDgotoxy(Seven, RowOne);
  lcd74595->LCD2n(mth);
  // lcd74595->LCDgotoxy(Nine, RowOne);
  lcd74595->LCDputs("/20");
  // lcd74595->LCDgotoxy(Twelve, RowOne);
  lcd74595->LCD2n(y);
  // lcd74595->LCDgotoxy(Fourteen, RowOne);
  lcd74595->LCDputs("   ");

  lcd74595->LCDgotoxy(Zero, RowTwo);
  lcd74595->LCDputs("Time: ");
  // lcd74595->LCDgotoxy(Six, RowTwo);
  lcd74595->LCD2n(h24);
  // lcd74595->LCDgotoxy(Eight, RowTwo);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Nine, RowTwo);
  lcd74595->LCD2n(min);
  // lcd74595->LCDgotoxy(Eleven, RowTwo);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Twelve, RowTwo);
  lcd74595->LCD2n(sec);
  // lcd74595->LCDgotoxy(Fourteen, RowTwo);
  lcd74595->LCDputs("   ");
}

void app::setup_time()
{
  lcd74595->LCDgotoxy(Zero, RowOne);
  lcd74595->LCDputs("    CAI DAT     ");
  lcd74595->LCDgotoxy(Zero, RowTwo);
  lcd74595->LCDputs("   THOI GIAN    ");
  delay(2000);
  lcd74595->LCDclear();
  show_DS();
  byte flagLoop = 1;
  ListSetupTime flagMoveLeftRight = (ListSetupTime)daye;
  UpDown flagUpDown = (UpDown)None;

  while (flagLoop == 1)
  {
    if (irrecv->decode(&results))
    {
      irrecv->resume();
      switch (results.value)
      {
      case ButtonUp:
      {
        flagUpDown = (UpDown)Up;
        break;
      }
      case ButtonDown:
      {
        flagUpDown = (UpDown)Down;
        break;
      }
      case ButtonLeft:
      {
        if (flagMoveLeftRight >= (ListSetupTime)daye)
          flagMoveLeftRight = ListSetupTime(flagMoveLeftRight - 1);
        if (flagMoveLeftRight < (ListSetupTime)daye)
          flagMoveLeftRight = (ListSetupTime)second;
        break;
      }
      case ButtonRight:
      {
        if (flagMoveLeftRight <= (ListSetupTime)second)
          flagMoveLeftRight = ListSetupTime(flagMoveLeftRight + 1);
        if (flagMoveLeftRight > (ListSetupTime)second)
          flagMoveLeftRight = (ListSetupTime)daye;
        break;
      }
      case ButtonOk:
      {
        flagLoop = 0;
        flagUpDown = (UpDown)Quit;
        flagMoveLeftRight = (ListSetupTime)quit;
        ds1307->DS_W(SEC, sec);  // sec
        ds1307->DS_W(MIN, min);  // min
        ds1307->DS_W(HOUR, h24); // h24
        ds1307->DS_W(DAY, day);  // dat
        ds1307->DS_W(DATE, dt);  // date
        ds1307->DS_W(MTH, mth);  // moth
        ds1307->DS_W(YEAR, y);   // year
        if (sec <= 12)
          preSec = sec + 11;
        else
          preSec = sec;
        preMin = min;
        preH24 = h24;
        tamp_day = 0;
        ds1307->DS_W(PreSecond, preSec);
        ds1307->DS_W(PreMinute, preMin);
        ds1307->DS_W(PreHour, preH24);
        ds1307->DS_W(Tampday, tamp_day);
        lcd74595->LCDclear();
        delay(200);

        lcd74595->LCDgotoxy(Zero, RowOne);
        lcd74595->LCDputs("    CAI DAT     ");
        lcd74595->LCDgotoxy(Zero, 1);
        lcd74595->LCDputs("   HOAN  TAT   ");
        delay(1000);
        lcd74595->LCDclear();
        tamp_sec = 0;
        flagDislayLCD = 1;
        read_time();
        break;
      }
      default:
        break;
      }
      delay(200);
    }
    switch (flagMoveLeftRight)
    {
    case (ListSetupTime)daye:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (day < dayMax)
          day++;
        else if (day >= dayMax)
          day = dayMin;
        lcd74595->LCDgotoxy(Zero, RowOne);
        lcd74595->LCDputs(day[thu]);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(0, 0);
        // lcd74595->LCDputs(day[thu]);
        break;
      }
      case (UpDown)Down:
      {
        if (day > dayMin)
          day--;
        else if (day == dayMin)
          day = dayMax;
        lcd74595->LCDgotoxy(Zero, RowOne);
        lcd74595->LCDputs(day[thu]);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (ListSetupTime)date:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (dt < dateMax)
          dt++;
        else if (dt >= dateMax)
          dt = dateMin;
        lcd74595->LCDgotoxy(Four, RowOne);
        lcd74595->LCD2n(dt);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(4, 0);
        // lcd74595->LCD2n(dt);
        break;
      }
      case (UpDown)Down:
      {
        if (dt > dateMin)
          dt--;
        else if (dt <= dateMin)
          dt = dateMax;
        lcd74595->LCDgotoxy(Four, RowOne);
        lcd74595->LCD2n(dt);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (ListSetupTime)month:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (mth < monthMax)
          mth++;
        else if (mth >= monthMax)
          mth = monthMin;
        lcd74595->LCDgotoxy(Seven, RowOne);
        lcd74595->LCD2n(mth);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(7, 0);
        // lcd74595->LCD2n(mth);
        break;
      }
      case (UpDown)Down:
      {
        if (mth > monthMin)
          mth--;
        else if (mth <= monthMin)
          mth = monthMax;
        lcd74595->LCDgotoxy(Seven, RowOne);
        lcd74595->LCD2n(mth);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (ListSetupTime)year:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (y < yearMax)
          y++;
        else if (y >= yearMax)
          y = yearMin;
        lcd74595->LCDgotoxy(Twelve, RowOne);
        lcd74595->LCD2n(y);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(12, 0);
        // lcd74595->LCD2n(y);
        break;
      }
      case (UpDown)Down:
      {
        if (y > yearMin)
          y--;
        else if (y <= yearMin)
          y = yearMax;
        lcd74595->LCDgotoxy(Twelve, RowOne);
        lcd74595->LCD2n(y);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (ListSetupTime)hour:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (h24 < hourMax)
          h24++;
        else if (h24 >= hourMax)
          h24 = hourMin;
        lcd74595->LCDgotoxy(Six, RowTwo);
        lcd74595->LCD2n(h24);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(6, 1);
        // lcd74595->LCD2n(h24);
        break;
      }
      case (UpDown)Down:
      {
        if (h24 > hourMin)
          h24--;
        else if (h24 <= hourMin)
          h24 = hourMax;
        lcd74595->LCDgotoxy(Six, RowTwo);
        lcd74595->LCD2n(h24);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
    }
    case (ListSetupTime)minute:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (min < minuteMax)
          min++;
        else if (min >= minuteMax)
          min = minuteMin;
        lcd74595->LCDgotoxy(Nine, RowTwo);
        lcd74595->LCD2n(min);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(9, 1);
        // lcd74595->LCD2n(min);
        break;
      }
      case (UpDown)Down:
      {
        if (min > minuteMin)
          min--;
        else if (min <= minuteMin)
          min = minuteMax;
        lcd74595->LCDgotoxy(Nine, RowTwo);
        lcd74595->LCD2n(min);
        flagUpDown = (UpDown)None;
        break;
      }
      default:
        break;
      }
      break;
    }
    case (ListSetupTime)second:
    {
      switch (flagUpDown)
      {
      case (UpDown)Up:
      {
        if (sec < secondMax)
          sec++;
        else if (sec >= secondMax)
          sec = secondMin;
        lcd74595->LCDgotoxy(Twelve, RowTwo);
        lcd74595->LCD2n(sec);
        flagUpDown = (UpDown)None;
        break;
      }
      case (UpDown)None:
      {
        // lcd74595->LCDgotoxy(12, 1);
        // lcd74595->LCD2n(sec);
        break;
      }
      case (UpDown)Down:
      {
        if (sec > secondMin)
          sec--;
        else if (sec <= secondMin)
          sec = secondMax;
        lcd74595->LCDgotoxy(Twelve, RowTwo);
        lcd74595->LCD2n(sec);
        flagUpDown = (UpDown)None;
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
  byte Second1 = ds1307->DS_r(inSecond1);
  byte Minute1 = ds1307->DS_r(inMinute1);
  byte Hour1 = ds1307->DS_r(inHour1);
  byte Switch1 = ds1307->DS_r(inSwitch1);

  byte Second2 = ds1307->DS_r(inSecond2);
  byte Minute2 = ds1307->DS_r(inMinute2);
  byte Hour2 = ds1307->DS_r(inHour2);
  byte Switch2 = ds1307->DS_r(inSwitch2);

  switch (flagMoveLeftRight)
  {
  case (ListSetupTimer)switch1:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      Switch1 = ON;
      ds1307->DS_W(inSwitch1, Switch1);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      Switch1 = OFF;
      ds1307->DS_W(inSwitch1, Switch1);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)hour1:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Hour1 < hourMax)
        Hour1++;
      else if (Hour1 >= hourMax)
        Hour1 = hourMin;
      ds1307->DS_W(inHour1, Hour1);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Hour1 > hourMin)
        Hour1--;
      else if (Hour1 == hourMin)
        Hour1 = hourMax;
      ds1307->DS_W(inHour1, Hour1);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)minute1:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Minute1 < minuteMax)
        Minute1++;
      else if (Minute1 >= minuteMax)
        Minute1 = minuteMin;
      ds1307->DS_W(inMinute1, Minute1);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Minute1 > minuteMin)
        Minute1--;
      else if (Minute1 == minuteMin)
        Minute1 = minuteMax;
      ds1307->DS_W(inMinute1, Minute1);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)second1:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Second1 < secondMax)
        Second1++;
      else if (Second1 >= secondMax)
        Second1 = secondMin;
      ds1307->DS_W(inSecond1, Second1);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Second1 > secondMin)
        Second1--;
      else if (Second1 == secondMin)
        Second1 = secondMax;
      ds1307->DS_W(inSecond1, Second1);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)switch2:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      Switch2 = ON;
      ds1307->DS_W(inSwitch2, Switch2);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      Switch2 = OFF;
      ds1307->DS_W(inSwitch2, Switch2);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)hour2:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Hour2 < hourMax)
        Hour2++;
      else if (Hour2 >= hourMax)
        Hour2 = hourMin;
      ds1307->DS_W(inHour2, Hour2);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Hour2 > hourMin)
        Hour2--;
      else if (Hour2 == hourMin)
        Hour2 = hourMax;
      ds1307->DS_W(inHour2, Hour2);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)minute2:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Minute2 < minuteMax)
        Minute2++;
      else if (Minute2 >= minuteMax)
        Minute2 = minuteMin;
      ds1307->DS_W(inMinute2, Minute2);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Minute2 > minuteMin)
        Minute2--;
      else if (Minute2 == minuteMin)
        Minute2 = minuteMax;
      ds1307->DS_W(inMinute2, Minute2);
      flagUpDown = (UpDown)None;
      break;
    }
    default:
      break;
    }
    break;
  }
  case (ListSetupTimer)second2:
  {
    switch (flagUpDown)
    {
    case (UpDown)Up:
    {
      if (Second2 < secondMax)
        Second2++;
      else if (Second2 >= secondMax)
        Second2 = secondMin;
      ds1307->DS_W(inSecond2, Second2);
      flagUpDown = (UpDown)None;
      break;
    }
    case (UpDown)Down:
    {
      if (Second2 > secondMin)
        Second2--;
      else if (Second2 == secondMin)
        Second2 = secondMax;
      ds1307->DS_W(inSecond2, Second2);
      flagUpDown = (UpDown)None;
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
  // flagMoveLeftRight = (ListSetupTimer)none;
  Second1 = ds1307->DS_r(inSecond1);
  Minute1 = ds1307->DS_r(inMinute1);
  Hour1 = ds1307->DS_r(inHour1);
  Switch1 = ds1307->DS_r(inSwitch1);

  Second2 = ds1307->DS_r(inSecond2);
  Minute2 = ds1307->DS_r(inMinute2);
  Hour2 = ds1307->DS_r(inHour2);
  Switch2 = ds1307->DS_r(inSwitch2);

  lcd74595->LCDgotoxy(Zero, RowOne);
  lcd74595->LCDputs("ON ");
  // lcd74595->LCDgotoxy(Three, RowOne);
  lcd74595->LCD1n(flagChooseLight + 1);
  if (Switch1 == ON)
  {
    // lcd74595->LCDgotoxy(Four, RowOne);
    lcd74595->LCDputs(" MO ");
  }
  else
  {
    // lcd74595->LCDgotoxy(Four, RowOne);
    lcd74595->LCDputs(" TAT");
  }
  // lcd74595->LCDgotoxy(Eight, RowOne);
  lcd74595->LCD2n(Hour1);
  // lcd74595->LCDgotoxy(Ten, RowOne);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Eleven, RowOne);
  lcd74595->LCD2n(Minute1);
  // lcd74595->LCDgotoxy(Thirteen, RowOne);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Fourteen, RowOne);
  lcd74595->LCD2n(Second1);

  lcd74595->LCDgotoxy(Zero, RowTwo);
  lcd74595->LCDputs("OFF");
  // lcd74595->LCDgotoxy(Three, RowTwo);
  lcd74595->LCD1n(flagChooseLight + 1);
  if (Switch2 == ON)
  {
    // lcd74595->LCDgotoxy(Four, RowTwo);
    lcd74595->LCDputs(" MO ");
  }
  else
  {
    // lcd74595->LCDgotoxy(Four, RowTwo);
    lcd74595->LCDputs(" TAT");
  }
  // lcd74595->LCDgotoxy(Eight, RowTwo);
  lcd74595->LCD2n(Hour2);
  // lcd74595->LCDgotoxy(Ten, RowTwo);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Eleven, RowTwo);
  lcd74595->LCD2n(Minute2);
  // lcd74595->LCDgotoxy(Thirteen, RowTwo);
  lcd74595->LCDputs(":");
  // lcd74595->LCDgotoxy(Fourteen, RowTwo);
  lcd74595->LCD2n(Second2);
}

void app::mode_set_timer_light()
{
  lcd74595->LCDclear();
  byte flagLoop = 1;
  OrderLight flagChooseLight = OrderLight1;
  ListSetupTimer flagChooseTypeTime = (ListSetupTimer)switch1;
  UpDown flagUpDown = (UpDown)None;
  byte flagDisplayLCD = 1;
  while (flagChooseLight != OrderEmpty)
  {
    while (flagLoop == 1)
    {
      if (flagDisplayLCD)
      {
        flagDisplayLCD = 0;
        lcd74595->LCDgotoxy(0, 0);
        lcd74595->LCDputs("CHON DEN CAI DAT:");
        lcd74595->LCDgotoxy(0, 1);
        lcd74595->LCDputs("  1   2   3   4  ");
      }

      if (irrecv->decode(&results))
      {
        irrecv->resume();
        switch (results.value) // press button
        {
        case Button1:
        {
          flagChooseLight = (OrderLight)OrderLight1;
          flagLoop = 2;
          flagChooseTypeTime = (ListSetupTimer)switch1;
          set_time(SecondFisrt_1, SecondFisrt_2, MinuteFisrt_1, MinuteFisrt_2, HourFisrt_1, HourFisrt_2, SwitchFirst_1, SwitchFirst_2, (OrderLight)OrderLight1, flagChooseTypeTime, flagUpDown);
          break;
        }
        case Button2:
        {
          flagChooseLight = (OrderLight)OrderLight2;
          flagLoop = 2;
          flagChooseTypeTime = (ListSetupTimer)switch1;
          set_time(SecondSecond_1, SecondSecond_2, MinuteSecond_1, MinuteSecond_2, HourSecond_1, HourSecond_2, SwitchSecond_1, SwitchSecond_2, (OrderLight)OrderLight2, flagChooseTypeTime, flagUpDown);
          break;
        }
        case Button3:
        {
          flagChooseLight = (OrderLight)OrderLight3;
          flagLoop = 2;
          flagChooseTypeTime = (ListSetupTimer)switch1;
          set_time(SecondThird_1, SecondThird_2, MinuteThird_1, MinuteThird_2, HourThird_1, HourThird_2, SwitchThird_1, SwitchThird_2, (OrderLight)OrderLight3, flagChooseTypeTime, flagUpDown);
          break;
        }
        case Button4:
        {
          flagChooseLight = (OrderLight)OrderLight4;
          flagLoop = 2;
          flagChooseTypeTime = (ListSetupTimer)switch1;
          set_time(SecondFourth_1, SecondFourth_2, MinuteFourth_1, MinuteFourth_2, HourFourth_1, HourFourth_2, SwitchFourth_1, SwitchFourth_2, (OrderLight)OrderLight4, flagChooseTypeTime, flagUpDown);
          break;
        }
        case ButtonBack:
        {
          flagChooseLight = OrderEmpty;
          flagLoop = 3;
          tamp_sec = 0;
          flagDislayLCD = 1;
          read_time_installed();
          lcd74595->LCDclear();
          break;
        }
        default:
          break;
        }
        flagDisplayLCD = 1;
      }
    }
    while (flagLoop == 2)
    {
      if (irrecv->decode(&results))
      {
        irrecv->resume();
        switch (results.value)
        {
        case ButtonUp:
        {
          flagUpDown = (UpDown)Up;
          break;
        }
        case ButtonDown:
        {
          flagUpDown = (UpDown)Down;
          break;
        }
        case ButtonLeft:
        {
          if (flagChooseTypeTime >= (ListSetupTimer)switch1)
            flagChooseTypeTime = ListSetupTimer(flagChooseTypeTime - 1);
          if (flagChooseTypeTime < (ListSetupTimer)switch1)
            flagChooseTypeTime = (ListSetupTimer)second2;
          break;
        }
        case ButtonRight:
        {
          if (flagChooseTypeTime <= (ListSetupTimer)second2)
            flagChooseTypeTime = ListSetupTimer(flagChooseTypeTime + 1);
          if (flagChooseTypeTime > (ListSetupTimer)second2)
            flagChooseTypeTime = (ListSetupTimer)switch1;
          break;
        }
        case ButtonOk:
        {
          flagLoop = 1;
          flagChooseLight = OrderOk;
          lcd74595->LCDclear();
          break;
        }
        default:
          break;
        }
        switch (flagChooseLight)
        {
        case (OrderLight)OrderLight1:
        {
          if (flagUpDown != (UpDown)None)
          {
            set_time(SecondFisrt_1, SecondFisrt_2, MinuteFisrt_1, MinuteFisrt_2, HourFisrt_1, HourFisrt_2, SwitchFirst_1, SwitchFirst_2, (OrderLight)OrderLight1, flagChooseTypeTime, flagUpDown);
            flagUpDown = (UpDown)None;
          }
          break;
        }
        case (OrderLight)OrderLight2:
        {
          if (flagUpDown != (UpDown)None || flagChooseTypeTime != (ListSetupTimer)none)
          {
            set_time(SecondSecond_1, SecondSecond_2, MinuteSecond_1, MinuteSecond_2, HourSecond_1, HourSecond_2, SwitchSecond_1, SwitchSecond_2, (OrderLight)OrderLight2, flagChooseTypeTime, flagUpDown);
            flagUpDown = (UpDown)None;
          }
          break;
        }
        case (OrderLight)OrderLight3:
        {
          if (flagUpDown != (UpDown)None || flagChooseTypeTime != (ListSetupTimer)none)
          {
            set_time(SecondThird_1, SecondThird_2, MinuteThird_1, MinuteThird_2, HourThird_1, HourThird_2, SwitchThird_1, SwitchThird_2, (OrderLight)OrderLight3, flagChooseTypeTime, flagUpDown);
            flagUpDown = (UpDown)None;
          }
          break;
        }
        case (OrderLight)OrderLight4:
        {
          if (flagUpDown != (UpDown)None || flagChooseTypeTime != (ListSetupTimer)none)
          {
            set_time(SecondFourth_1, SecondFourth_2, MinuteFourth_1, MinuteFourth_2, HourFourth_1, HourFourth_2, SwitchFourth_1, SwitchFourth_2, (OrderLight)OrderLight4, flagChooseTypeTime, flagUpDown);
            flagUpDown = (UpDown)None;
          }
          break;
        }
        default:
          break;
        }
      }
    }
  }
}

void app::mode_time_adjustment()
{
  if ((previousDay > day || previousDay < day) && tamp_day < NumberDayAdjustment)
  {
    tamp_day++;
    ds1307->DS_W(Tampday, tamp_day);
    previousDay = day;
  }
  if (tamp_day == NumberDayAdjustment && preSec == sec && preMin == min && preH24 == h24)
  {
    sec = sec - 11; // tolerance time
    ds1307->DS_W(SEC, sec);
    tamp_day = 0;
    ds1307->DS_W(Tampday, tamp_day);
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
    lcd74595->LCDclear();
  }
}

void app::mode_on_off()
{
  // Using relay SSR Low Level Trigger

  //***********On light 1*******************
  if (sw1_1 == ON)
  {
    if ((hour1_1 == h24) && (minute1_1 == min) && (second1_1 == sec))
    {
      digitalWrite(Light1, LOW);
      stateLight1 = OFF;
    }
  }

  //**********Off light 1*****************
  if (sw1_2 == ON)
  {
    if ((hour1_2 == h24) && (minute1_2 == min) && (second1_2 == sec))
    {
      digitalWrite(Light1, HIGH);
      stateLight1 = ON;
    }
  }

  //***********On light 2*******************
  if (sw2_1 == ON)
  {
    if ((hour2_1 == h24) && (minute2_1 == min) && (second2_1 == sec))
    {
      digitalWrite(Light2, LOW);
      stateLight2 = OFF;
    }
  }

  //**********Off light 2*****************
  if (sw2_2 == ON)
  {
    if ((hour2_2 == h24) && (minute2_2 == min) && (second2_2 == sec))
    {
      digitalWrite(Light2, HIGH);
      stateLight2 = ON;
    }
  }

  //***********On light 3*******************
  if (sw3_1 == ON)
  {
    if ((hour3_1 == h24) && (minute3_1 == min) && (second3_1 == sec))
    {
      digitalWrite(Light3, LOW);
      stateLight3 = OFF;
    }
  }

  //**********Off light 3*****************
  if (sw3_2 == ON)
  {
    if ((hour3_2 == h24) && (minute3_2 == min) && (second3_2 == sec))
    {
      digitalWrite(Light3, HIGH);
      stateLight3 = ON;
    }
  }

  //***********On light 4*******************
  if (sw4_1 == ON)
  {
    if ((hour4_1 == h24) && (minute4_1 == min) && (second4_1 == sec))
    {
      digitalWrite(Light4, LOW);
      stateLight4 = OFF;
    }
  }

  //**********Off light 4*****************
  if (sw4_2 == ON)
  {
    if ((hour4_2 == h24) && (minute4_2 == min) && (second4_2 == sec))
    {
      digitalWrite(Light4, HIGH);
      stateLight4 = ON;
    }
  }
}

void app::mode_receive_IR()
{
  if (irrecv->decode(&results))
  {
    irrecv->resume();
    switch (results.value)
    {
    case ButtonApp:
    {
      setup_time();
      break;
    }
    case ButtonMenu:
    {
      mode_set_timer_light();
      break;
    }
    case Button1:
    {
      stateLight1 = !stateLight1;
      digitalWrite(Light1, stateLight1);
      break;
    }
    case Button2:
    {
      stateLight2 = !stateLight2;
      digitalWrite(Light2, stateLight2);
      break;
    }
    case Button3:
    {
      stateLight3 = !stateLight3;
      digitalWrite(Light3, stateLight3);
      break;
    }
    case Button4:
    {
      stateLight4 = !stateLight4;
      digitalWrite(Light4, stateLight4);
      break;
    }
    case Button5:
    {
      stateLightLCD = !stateLightLCD;
      digitalWrite(LightLCD, stateLightLCD);
      break;
    }
    case Button6:
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

void app::read_time_installed()
{
  second1_1 = ds1307->DS_r(SecondFisrt_1);
  minute1_1 = ds1307->DS_r(MinuteFisrt_1);
  hour1_1 = ds1307->DS_r(HourFisrt_1);
  sw1_1 = ds1307->DS_r(SwitchFirst_1);

  second1_2 = ds1307->DS_r(SecondFisrt_2);
  minute1_2 = ds1307->DS_r(MinuteFisrt_2);
  hour1_2 = ds1307->DS_r(HourFisrt_2);
  sw1_2 = ds1307->DS_r(SwitchFirst_2);

  second2_1 = ds1307->DS_r(SecondSecond_1);
  minute2_1 = ds1307->DS_r(MinuteSecond_1);
  hour2_1 = ds1307->DS_r(HourSecond_1);
  sw2_1 = ds1307->DS_r(SwitchSecond_1);

  second2_2 = ds1307->DS_r(SecondSecond_2);
  minute2_2 = ds1307->DS_r(MinuteSecond_2);
  hour2_2 = ds1307->DS_r(HourSecond_2);
  sw2_2 = ds1307->DS_r(SwitchSecond_2);

  second3_1 = ds1307->DS_r(SecondThird_1);
  minute3_1 = ds1307->DS_r(MinuteThird_1);
  hour3_1 = ds1307->DS_r(HourThird_1);
  sw3_1 = ds1307->DS_r(SwitchThird_1);

  second3_2 = ds1307->DS_r(SecondThird_2);
  minute3_2 = ds1307->DS_r(MinuteThird_2);
  hour3_2 = ds1307->DS_r(HourThird_2);
  sw3_2 = ds1307->DS_r(SwitchThird_2);

  second4_1 = ds1307->DS_r(SecondFourth_1);
  minute4_1 = ds1307->DS_r(MinuteFourth_1);
  hour4_1 = ds1307->DS_r(HourFourth_1);
  sw4_1 = ds1307->DS_r(SwitchFourth_1);

  second4_2 = ds1307->DS_r(SecondFourth_2);
  minute4_2 = ds1307->DS_r(MinuteFourth_2);
  hour4_2 = ds1307->DS_r(HourFourth_2);
  sw4_2 = ds1307->DS_r(SwitchFourth_2);
}

void app::init_interrupt()
{
  pinMode(ButtonLight1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ButtonLight1), onOffLight1, FALLING);
  pinMode(ButtonLight2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ButtonLight2), onOffLight2, FALLING);
  pinMode(ButtonLight3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ButtonLight3), onOffLight3, FALLING);
}

void app::print_hello()
{
  lcd74595->LCDgotoxy(Zero, RowOne);
  lcd74595->LCDputs("    WELCOME     ");
  lcd74595->LCDgotoxy(Zero, RowTwo);
  lcd74595->LCDputs("KHAI'S EQUIPMENT");
  delay(1000);
  digitalWrite(LightLCD, LOW);
  lcd74595->LCDclear();
}

void app::read_time()
{
  sec = ds1307->DS_r(SEC);
  min = ds1307->DS_r(MIN);
  h24 = ds1307->DS_r(HOUR);
  day = ds1307->DS_r(DAY);
  dt = ds1307->DS_r(DATE);
  mth = ds1307->DS_r(MTH);
  y = ds1307->DS_r(YEAR);
}