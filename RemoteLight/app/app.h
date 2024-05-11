#ifndef APP_H_
#define APP_H_

#include "../ds1307/TypeDef.h"
#include "../lcd74595/lcd74595.h"
#include "../ds1307/ds1307.h"
#include "../IRremote/IRremote.h"

class app
{
public:
    app() : lcd74595(P1_0, P1_1, P1_2),
            ds1307(P1_6, P1_7),
            irrecv(P1_5){};
    void init();
    void run();
    void config();

private:
    void read_DS();
    void show_DS();
    void setup_time();
    void set_time(unsigned char inSecond1, unsigned char inSecond2, unsigned char inMinute1, unsigned char inMinute2, unsigned char inHour1, unsigned char inHour2, unsigned char inSwitch1, unsigned char inSwitch2, OrderLight flagChooseLight, ListSetupTimer &flagMoveLeftRight, UpDown &flagUpDown);
    void mode_set_timer_light();
    void mode_time_adjustment();
    void mode_clear_lcd_display();
    void mode_on_off();
    static void onOffLight1();
    static void onOffLight2();
    static void onOffLight3();

    LCD74595 lcd74595;
    DS1307 ds1307;
    IRrecv irrecv;
    decode_results results;
    byte sec;
    byte h24;
    byte min;
    byte day;
    byte dt;
    byte mth;
    byte y;
    byte preSec;
    byte preMin;
    byte preH24;
    byte tamp_day;
    byte previousDay;
    byte tamp_sec;
    byte previousSec;
    byte flagDislayLCD;
    byte variabilitySecond;
    byte variabilityMinute;
    byte variabilityHour;
    static bool stateLight1;
    static bool stateLight2;
    static bool stateLight3;
    static bool stateLight4;
    static bool stateLightLCD;
};

#endif