#include <PAJ7620.h>
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>

#define PAJ7620_Delay 800

// 初始化 U8G2 库，这里假设使用的是 SSD1306 驱动的 128x64 OLED 屏幕
// 使用自定义的 I2C 引脚
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* SCL=*/ 5, /* SDA=*/ 4, /* reset=*/ U8X8_PIN_NONE);

void setup()
{
    uint8_t error = 0;

    Serial.begin(115200);
    Serial.println("\r\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

    error = PAJ7620Init(); // initialize Paj7620 registers
    if (error)
    {
        Serial.print("INIT ERROR,CODE:");
        Serial.println(error);
    }
    else
    {
        Serial.println("INIT OK");
    }
    Serial.println("Please input your gestures:\n");

    // 初始化 OLED 屏幕
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
    u8g2.drawStr(0, 15, "Gesture Recognition");
    u8g2.sendBuffer();
}

void loop()
{
    uint8_t data = 0;

    PAJ7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    switch (data) // When different gestures be detected, the variable 'data' will be set to different values by PAJ7620ReadReg(0x43, 1, &data).
    {
    case GES_RIGHT_FLAG:
        delay(PAJ7620_Delay);
        PAJ7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG)
        {
            u8g2.print("Forward");
            delay(PAJ7620_Delay);
        }
        else if (data == GES_BACKWARD_FLAG)
        {
            u8g2.print("Backward");
            delay(PAJ7620_Delay);
        }
        else
            u8g2.print("Right");
        break;

    case GES_LEFT_FLAG:
        delay(PAJ7620_Delay);
        PAJ7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG)
        {
            u8g2.print("Forward");
            delay(PAJ7620_Delay);
        }
        else if (data == GES_BACKWARD_FLAG)
        {
            u8g2.print("Backward");
            delay(PAJ7620_Delay);
        }
        else
            u8g2.print("Left");
        break;

    case GES_UP_FLAG:
        delay(PAJ7620_Delay);
        PAJ7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG)
        {
            u8g2.print("Forward");
            delay(PAJ7620_Delay);
        }
        else if (data == GES_BACKWARD_FLAG)
        {
            u8g2.print("Backward");
            delay(PAJ7620_Delay);
        }
        else
            u8g2.print("Up");
        break;

    case GES_DOWN_FLAG:
        delay(PAJ7620_Delay);
        PAJ7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG)
        {
            u8g2.print("Forward");
            delay(PAJ7620_Delay);
        }
        else if (data == GES_BACKWARD_FLAG)
        {
            u8g2.print("Backward");
            delay(PAJ7620_Delay);
        }
        else
            u8g2.print("Down");
        break;

    case GES_FORWARD_FLAG:
        u8g2.print("Forward");
        delay(PAJ7620_Delay);
        break;

    case GES_BACKWARD_FLAG:
        u8g2.print("Backward");
        delay(PAJ7620_Delay);
        break;

    case GES_CLOCKWISE_FLAG:
        u8g2.print("Clockwise");
        break;

    case GES_COUNT_CLOCKWISE_FLAG:
        u8g2.print("Anti-clockwise");
        break;

    default:
        PAJ7620ReadReg(0x44, 1, &data);
        if (data == GES_WAVE_FLAG)
            u8g2.print("Wave");
        break;
    }
    u8g2.sendBuffer();
    delay(100);
}
