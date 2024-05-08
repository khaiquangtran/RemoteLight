enum TimeStore : byte
{
    SEC = 0x00,
    MIN,
    HOUR,
    DAY,
    DATE,
    MTH,
    YEAR
};

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

struct Light
{
    const byte Light1 = P2_3;
    const byte Light2 = P2_4;
    const byte Light3 = P2_5;
    const byte Light4 = P2_6;
    const byte LightLCD = P2_7;

    const byte ButtonLight1 = P2_0;
    const byte ButtonLight2 = P2_1;
    const byte ButtonLight3 = P2_2;
};

enum ButtonCode : unsigned long
{
    Button1 = 0xFF807F,
    Button2 = 0xFF40BF,
    Button3 = 0xFFC03F,
    Button4 = 0xFF20DF,
    Button5 = 0xFFA05F,
    Button6 = 0xFF609F,

    ButtonUp = 0xFFE21D,
    ButtonDown = 0xFFD22D,
    ButtonRight = 0xFF52AD,
    ButtonLeft = 0xFF12ED,

    ButtonOk = 0xFF926D,
    ButtonMenu = 0xFFB24D,
    ButtonApp = 0xFF6A95
}

enum ListSetupTime : byte
{
    day = 0x00,
    date = 0x01,
    month = 0x02,
    year = 0x03,
    hour = 0x04,
    minute = 0x05,
    second = 0x06,
    quit = 0x07
}

enum ListSetupTimer : byte
{
    switch1 = 0x00,
    hour1 = 0x01,
    minute1 = 0x02,
    second1 = 0x03,
    switch2 = 0x04,
    hour2 = 0x05,
    minute2 = 0x06,
    second2 = 0x07,
}

enum UpDown : byte
{
    Quit = 0x04,
    Up = 0x03,
    None = 0x02,
    Down = 0x01
}

#define secondMax 59
#define secondMin 0

#define minuteMax 59
#define minuteMin 0

#define hourMax 23
#define hourMin 0

#define yearMax 99
#define yearMin 0

#define monthMax 12
#define monthMin 0

#define dateMax 31
#define dateMin 0

#define dayMax 7
#define dayMin 1

#define ON 1
#define OFF 0

enum Row : byte
{
    One = 0x00,
    Two = 0x01
}

enum Column : byte
{
    Zero = 0x00,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Eleven,
    Twelve,
    Thirteen,
    Fourteen,
    Fifteen,
    Sixteen
}