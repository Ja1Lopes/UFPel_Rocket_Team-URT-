#include "HX711.h"
#include <SD.h>
#include <SPI.h>

#define DT A1
#define SCK A0

const float scale = 102234.25;
const int chipSelect = 10;

File dataFile;
HX711 scaleSensor;

void (*resetFunc)(void) = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    delay(2000);
    resetFunc();
  }

  scaleSensor.begin(DT, SCK);
  Serial.print("Tare Reading: ");
  Serial.println(scaleSensor.read());
  delay(1000);

  scaleSensor.set_scale(scale);
  scaleSensor.tare();
  Serial.println("Place the item to weigh.");
}

void loop()
{
  Serial.print("Kg: ");
  float weight = scaleSensor.get_units(10);
  Serial.print(weight);
  Serial.print("  |  Raw Value: ");
  int rawValue = scaleSensor.get_value(10);
  Serial.println(rawValue);

  String data = String(weight) + "," + String(rawValue);
  datalog(data);

  delay(100);
}

void datalog(String dataString)
{
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
  else
  {
    Serial.println("error opening datalog.txt");
  }
  dataFile.close();
}
