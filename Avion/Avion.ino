#include <GY521_registers.h>
#include <GY521.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

Adafruit_BMP085 bmp;

GY521 sensor(0x68);

#define buzz 3

unsigned long currentMillis = 0;
unsigned long intervalData = 0;

float temperatura, altitude, pressao;
float pitch, roll, yaw;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    delay(100);

    if (!bmp.begin())
    {
        debug(2);
        while (1)
        {
        }
    }

    while (sensor.wakeup() == false)
    {
        debug(3);
    }
    sensor.setAccelSensitivity(2); // 8g
    sensor.setGyroSensitivity(1);  // 500 degrees/s
    sensor.setThrottle();

    sensor.axe = 0.574;
    sensor.aye = -0.002;
    sensor.aze = -1.043;
    sensor.gxe = 10.702;
    sensor.gye = -6.436;
    sensor.gze = -0.676;

    pinMode(buzz, OUTPUT);

    debug(1);
}

void loop()
{
    currentMillis = millis();

    if (currentMillis - intervalData > 100)
    {
        sensor.read();
        temperatura = bmp.readTemperature();
        altitude = bmp.readAltitude();
        pressao = bmp.readPressure();
        pitch = sensor.getPitch();
        roll = sensor.getRoll();
        yaw = sensor.getYaw();
        int meds[6] = {temperatura, altitude, pressao, pitch, roll, yaw};
        for (int i = 0; i < sizeof(meds); i++)
        {
            Serial.println(meds[i]);
        }
    }
}

void debug(int n)
{
    if (n == 1)
    {
        digitalWrite(buzz, HIGH);
        delay(1000);
        digitalWrite(buzz, LOW);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            digitalWrite(buzz, HIGH);
            delay(500);
            digitalWrite(buzz, LOW);
        }
    }
}
