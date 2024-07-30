#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#define N(x) x + 1

#define SEC     0x00
#define MIN     N(SEC)
#define HOUR    N(MIN)
#define DAY     N(HOUR)
#define DATE    N(DAY)
#define MTH     N(DATE)
#define YEAR    N(MTH)

#define SecondFisrt_1   0x08
#define MinuteFisrt_1   N(SecondFisrt_1)
#define HourFisrt_1     N(MinuteFisrt_1)
#define SwitchFirst_1   N(HourFisrt_1)

#define SecondFisrt_2   0x0C
#define MinuteFisrt_2   N(SecondFisrt_2)
#define HourFisrt_2     N(MinuteFisrt_2)
#define SwitchFirst_2   N(HourFisrt_2)

#define SecondSecond_1  0x10
#define MinuteSecond_1  N(SecondSecond_1)
#define HourSecond_1    N(MinuteSecond_1)
#define SwitchSecond_1  N(HourSecond_1)

#define SecondSecond_2  0x14
#define MinuteSecond_2  N(SecondSecond_2)
#define HourSecond_2    N(MinuteSecond_2)
#define SwitchSecond_2  N(HourSecond_2)

#define SecondThird_1   0x18
#define MinuteThird_1   N(SecondThird_1)
#define HourThird_1     N(MinuteThird_1)
#define SwitchThird_1   N(HourThird_1)

#define SecondThird_2   0x1C
#define MinuteThird_2   N(SecondThird_2)
#define HourThird_2     N(MinuteThird_2)
#define SwitchThird_2   N(HourThird_2)

#define SecondFourth_1  0x20
#define MinuteFourth_1  N(SecondFourth_1)
#define HourFourth_1    N(MinuteFourth_1)
#define SwitchFourth_1  N(HourFourth_1)

#define SecondFourth_2  0x24
#define MinuteFourth_2  N(SecondFourth_2)
#define HourFourth_2    N(MinuteFourth_2)
#define SwitchFourth_2  N(HourFourth_2)

#define PreSecond   0x2C
#define PreMinute   N(PreSecond)
#define PreHour     N(PreMinute)
#define Tampday     N(PreHour)

#define Light1      P2_3
#define Light2      P2_4
#define Light3      P2_5
#define Light4      P2_6
#define LightLCD    P2_7

#define ButtonLight1 P2_0
#define ButtonLight2 P2_1
#define ButtonLight3 P2_2

#define Button1 0xFF807F
#define Button2 0xFF40BF
#define Button3 0xFFC03F
#define Button4 0xFF20DF
#define Button5 0xFFA05F
#define Button6 0xFF609F

#define ButtonUp    0xFFE21D
#define ButtonDown  0xFFD22D
#define ButtonRight 0xFF52AD
#define ButtonLeft  0xFF12ED

#define ButtonOk    0xFF926D
#define ButtonMenu  0xFFB24D
#define ButtonApp   0xFF6A95
#define ButtonBack  0xFF32CD

enum ListSetupTime : byte
{
    daye    = 0x00,
    date    = 0x01,
    month   = 0x02,
    year    = 0x03,
    hour    = 0x04,
    minute  = 0x05,
    second  = 0x06,
    quit    = 0x07
};

enum ListSetupTimer : byte
{
    switch1     = 0x00,
    hour1       = 0x01,
    minute1     = 0x02,
    second1     = 0x03,
    switch2     = 0x04,
    hour2       = 0x05,
    minute2     = 0x06,
    second2     = 0x07,
    none        = 0x0F
};

enum OrderLight : byte
{
    OrderLight1 = 0,
    OrderLight2,
    OrderLight3,
    OrderLight4,
    OrderEmpty,
    OrderOk
};

enum UpDown : byte
{
    Quit    = 0x04,
    Up      = 0x03,
    None    = 0x02,
    Down    = 0x01
};

#define secondMax 59
#define secondMin 0

#define minuteMax 59
#define minuteMin 0

#define hourMax 23
#define hourMin 0

#define yearMax 99
#define yearMin 0

#define monthMax 12
#define monthMin 1

#define dateMax 31
#define dateMin 1

#define dayMax 7
#define dayMin 1

#define ON 1
#define OFF 0

#define NumberDayAdjustment 1

#define RowOne  0x00
#define RowTwo  0x01

#define Zero        0x00
#define One         N(Zero)
#define Two         N(One)
#define Three       N(Two)
#define Four        N(Three)
#define Five        N(Four)
#define Six         N(Five)
#define Seven       N(Six)
#define Eight       N(Seven)
#define Nine        N(Eight)
#define Ten         N(Nine)
#define Eleven      N(Ten)
#define Twelve      N(Eleven)
#define Thirteen    N(Twelve)
#define Fourteen    N(Thirteen)
#define Fifteen     N(Fourteen)
#define Sixteen     N(Fifteen)

#endif