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


char flag;
void setup()
{
  // put your setup code here, to run once:

}
void loop()
{
  // put your main code here, to run repeatedly:

  
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
