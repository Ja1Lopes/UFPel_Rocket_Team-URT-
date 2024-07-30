#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

// Variaveis para receber os valores de aceleração dos eixos
int valoranteriorX = 0;
int valoranteriorY = 0;
int valoranteriorZ = 0;

unsigned long currentMillis = 0;
unsigned long intervalData = 0;

float altitude, pressao;

void setup(void)
{
    Serial.begin(115200); // iniciar o monitor serial
    Wire.begin();

    // Verifica se o módulo GY-521 (MPU6050) está conectado
    if (!mpu.begin())
    {
        Serial.println("Falha ao conectar o módulo"); // caso não encontre
        while (1)
        {
            delay(10);
        }
    }
    // Verifica se módulo BMP está conectado
    if (!bmp.begin())
    {
        while (1)
        {
        }
    }
    Serial.println("Módulo conectado"); // caso encontre
    // Definição da variação do chip.
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Variação do aceleremetro para: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Variação do Giroscópio para: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filtro: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    Serial.println("");
    delay(100);
}

void loop()
{
    // Variáveis para leitura do Acelerometro, Giroscopio e Tempratura.
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Imprime os valores do acelerometro no monitor serial
    Serial.print("Aceleração X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");
    // Imprime os valores do giroscopio no monitor serial
    Serial.print("Rotação X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");
    // Imprime os valores do termometro no monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temp.temperature);
    Serial.println(" °C");

    Serial.print("Altitude: ");
    Serial.print(bmp.readAltitude());
    Serial.println("m");

    Serial.print("Pressão: ");
    Serial.print(bmp.readPressure());
    Serial.println("Pa");

    Serial.println("");
    delay(500); // Atraso definido em milisegundos entre as leituras
}