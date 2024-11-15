#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#define SDCS 5
#define triggerChute 16
#define led 14

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

int valorAnteriorAltitude = 0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long chuteLastTriggered = 0;

void setup(void)
{

    Serial.begin(115200); // iniciar o monitor serial

    Wire.begin();
    pinMode(triggerChute, OUTPUT);
    digitalWrite(triggerChute, LOW);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    if (!SD.begin(SDCS))
    {
        Serial.println(F("Leitura Falhou"));
        debug(1);
        while (1)
            ;
    }
    // Verifica se o módulo GY-521 (MPU6050) está conectado
    if (!mpu.begin())
    {
        Serial.println("Falha ao conectar o módulo"); // caso não encontre
        while (1)
        {
            delay(10);
        }
        debug(1);
    }
    // Verifica se módulo BMP está conectado
    if (!bmp.begin())
    {
        debug(1);
    }
    Serial.println("Módulo conectado"); // caso encontre
    // Definição da variação do chip.
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("");
    debug(5);
}

void loop()
{
    currentMillis = millis();
    if (currentMillis - previousMillis > 100)
    {
        String data_string = "";
        float gyroX = 0;
        float gyroY = 0;
        float gyroZ = 0;
        float accelX = 0;
        float accelY = 0;
        float accelZ = 0;
        int pressure = 0;
        int altitude = 0;

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        accelX = a.acceleration.x;
        accelY = a.acceleration.y;
        accelZ = a.acceleration.z;
        gyroX = g.gyro.x;
        gyroY = g.gyro.y;
        gyroZ = g.gyro.z;
        pressure = bmp.readPressure();
        altitude = bmp.readAltitude(round(pressure * 1.001));
        float smoothedAltitude = smoothAltitude(altitude);

        Serial.print("Alt: ");
        Serial.println(smoothedAltitude);

        Serial.println(valorAnteriorAltitude);
        Serial.println(currentMillis);
        Serial.println(previousMillis);

        float descentRate = ((valorAnteriorAltitude - smoothedAltitude) / ((currentMillis - previousMillis) / 1000.0));
        if (descentRate > 0)
        {
            Serial.print("Descending at rate: ");
            Serial.println(descentRate);
        }
        else
        {
            Serial.print("Ascending at rate: ");
            Serial.println(-descentRate); // Print ascent rate as a positive number
        }
        Serial.println(accelZ);

        if (descentRate > 10.0 && (currentMillis - chuteLastTriggered) > 5000)
        {
            digitalWrite(triggerChute, HIGH);
            chuteLastTriggered = currentMillis;
            Serial.println("Parachute deployed!");
            debug(2);
        }
        else if ((accelZ < -5) && (currentMillis - chuteLastTriggered) > 5000)
        {
            digitalWrite(triggerChute, HIGH);
            chuteLastTriggered = currentMillis;
            Serial.println("Parachute deployed!");
            debug(2);
        }

        valorAnteriorAltitude = smoothedAltitude;

        /*if (altitude < valorAnteriorAltitude)
        {
            digitalWrite(triggerChute, HIGH);
            Serial.println("Paracaidas");
            debug(2);
        }
        valorAnteriorAltitude = altitude - 2;
        Serial.print("Alt_Control: ");
        Serial.println(valorAnteriorAltitude);*/

        data_string += String(accelX);
        data_string += "|";
        data_string += String(accelY);
        data_string += "|";
        data_string += String(accelZ);
        data_string += "|";
        data_string += String(gyroX);
        data_string += "|";
        data_string += String(gyroY);
        data_string += "|";
        data_string += String(gyroZ);
        data_string += "|";
        data_string += String(pressure);
        data_string += "|";
        data_string += String(altitude);

        logging(data_string);
        previousMillis = currentMillis;
    }
}

void logging(String data)
{
    File dataFile = SD.open("/dados.txt", FILE_APPEND);
    if (dataFile)
    {
        dataFile.println(data);
        dataFile.close();
    }
    else
    {
        Serial.println("Failed to write to SD card!");
        debug(3); // Indicate SD error
    }
}

void debug(int qtd)
{
    if (qtd == 1)
    {
        digitalWrite(led, HIGH);
        while (1)
            ;
    }
    int i = 0;
    while (i < qtd)
    {
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        i++;
        delay(500);
    }
}

const int smoothingWindow = 3;
float altitudeBuffer[smoothingWindow] = {0};
int bufferIndex = 0;

float smoothAltitude(float newAltitude)
{
    altitudeBuffer[bufferIndex] = newAltitude;
    bufferIndex = (bufferIndex + 1) % smoothingWindow;

    float sum = 0;
    for (int i = 0; i < smoothingWindow; i++)
    {
        sum += altitudeBuffer[i];
    }
    return sum / smoothingWindow;
}