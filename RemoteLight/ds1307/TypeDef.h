enum TimeStore : byte
{
    SEC = 0x00,
    MIN,
    HOUR,
    DAY,
    DATE,
    MTH,
    YEAR
}

enum SetupStore : byte
{
    SecondFisrt_1 = 0x08,
    MinuteFisrt_1 = 0x09,
    HourFisrt_1 = 0x0A,
    SwitchFirst_1 = 0x0B,

    SecondFisrt_2 = 0x0C,
    MinuteFisrt_2 = 0x0D,
    HourFisrt_2 = 0x0E,
    SwitchFirst_2 = 0x0F,

    SecondSecond_1 = 0x10,
    MinuteSecond_1 = 0x11,
    HourSecond_1 = 0x12,
    SwitchSecond_1 = 0x13,

    SecondSecond_2 = 0x14,
    MinuteSecond_2 = 0x15,
    HourSecond_2 = 0x16,
    SwitchSecond_2 = 0x17,

    SecondThird_1 = 0x18,
    MinuteThird_1 = 0x19,
    HourThird_1 = 0x1A,
    SwitchThird_1 = 0x1B,

    SecondThird_2 = 0x1C,
    MinuteThird_2 = 0x1D,
    HourThird_2 = 0x1E,
    SwitchThird_2 = 0x1F,

    SecondFourth_1 = 0x20,
    MinuteFourth_1 = 0x21,
    HourFourth_1 = 0x22,
    SwitchFourth_1 = 0x23,

    SecondFourth_2 = 0x24,
    MinuteFourth_2 = 0x25,
    HourFourth_2 = 0x26,
    SwitchFourth_2 = 0x27,

    PreSecond = 0x2C,
    PreMinute = 0x2D,
    PreHour = 0x2E,
    Tampday = 0x2f,
};

const byte Light1 = P2_3;
const byte Light2 = P2_4;
const byte Light3 = P2_5;
const byte Light4 = P2_6;
const byte LightLCD = P2_7;

const byte ButtonLight1 = P2_0;
const byte ButtonLight2 = P2_1;
const byte ButtonLight3 = P2_2;