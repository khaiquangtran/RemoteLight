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
    void set_time();
    LCD74595 lcd74595;
    DS1307 ds1307;
    IRrecv irrecv;
    decode_results results;
    Light light;
    char sec;
    char h24;
    char min;
    char day;
    char dt;
    char mth;
    char y;
    char preSec;
    char preMin;
    char preH24;
    char tamp_day;
    char previousDay;
    char tamp_sec;
    char previousSec;
}