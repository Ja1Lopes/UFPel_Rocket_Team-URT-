# 1 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino"
# 2 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2
# 3 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2
# 4 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2
# 5 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2
# 6 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2
# 7 "/home/lopes/Documents/GitHub/UFPel_Rocket_Team-URT-/Avion/Avion.ino" 2





Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

// Variaveis para receber os valores de aceleração dos eixos
int valorAnteriorAltitude = 0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup(void)
{

    Serial0.begin(115200); // iniciar o monitor serial

    Wire.begin();
    pinMode(16, 0x03);
    digitalWrite(16, 0x0);
    pinMode(14, 0x03);
    digitalWrite(14, 0x0);

    if (!SD.begin(5))
    {
        Serial0.println(((reinterpret_cast<const __FlashStringHelper *>(("Leitura Falhou")))));
        debug(1);
        while (1)
            ;
    }
    // Verifica se o módulo GY-521 (MPU6050) está conectado
    if (!mpu.begin())
    {
        Serial0.println("Falha ao conectar o módulo"); // caso não encontre
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
    Serial0.println("Módulo conectado"); // caso encontre
    // Definição da variação do chip.
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial0.println("");
    debug(5);
}

void loop()
{
    currentMillis = millis();
    if (currentMillis - previousMillis > 100)
    {
        previousMillis = currentMillis;
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
        altitude = bmp.readAltitude();
        if (altitude < (valorAnteriorAltitude))
        {
            digitalWrite(16, 0x1);
        }
        valorAnteriorAltitude = altitude - 10;

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
    }
}

void logging(String data)
{
    File dataFile = SD.open("/dados.txt", "a");
    if (dataFile)
    {
        dataFile.println(data);
        dataFile.close();
    }
}

void debug(int qtd)
{
    if (qtd == 1)
    {
        digitalWrite(14, 0x1);
        while (1)
            ;
    }
    int i = 0;
    while (i < qtd)
    {
        digitalWrite(14, 0x1);
        delay(500);
        digitalWrite(14, 0x0);
        i++;
        delay(500);
    }
}
