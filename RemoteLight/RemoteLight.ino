#include "app/app.h"
#include "app/app.cpp"
#include "ds1307/ds1307.h"
#include "ds1307/ds1307.cpp"
#include "lcd74595/lcd74595.h"
#include "lcd74595/lcd74595.cpp"
#include "IRremote/IRremote.h"
#include "IRremote/IRremote.cpp"

app app1;

void setup()
{
  // put your setup code here, to run once:
  app1.config();
  app1.init();
}
void loop()
{
  // put your main code here, to run repeatedly:
  app1.run();
}
