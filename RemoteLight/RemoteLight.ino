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

void setup()
{
  // put your setup code here, to run once:

}
void loop()
{
  // put your main code here, to run repeatedly:

  
}

