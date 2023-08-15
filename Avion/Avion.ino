#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <SD.h>
#include <SPI.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define buzz 3

#define SDModule 4

DHT dht(DHTPIN, DHTTYPE);

unsigned long currentMillis = 0;
unsigned long intervalData = 0;

float h, t, hic;

void setup()
{
    Serial.begin(9600);

    pinMode(buzz, OUTPUT);

    if (!SD.begin(SDModule))
    {
        Serial.println("Card failed, or not present");
        debug(2);
    }
    dht.begin();

    debug(1);
}

void loop()
{
    currentMillis = millis();

    if (currentMillis - intervalData > 100)
    {
        h = dht.readHumidity();
        t = dht.readTemperature();
        hic = dht.computeHeatIndex(t, h, false);

        
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
