#include "HX711.h"
#include <SD.h>
#include <SPI.h>

#define DT A1
#define SCK A0

const float scale = 102234.25;
const int chipSelect = 4;

HX711 scaleSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    while (1)
      ;
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
  secondDatalog(data);

  delay(100);
}

void datalog(String dataString)
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
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
}

void secondDatalog(String dataString)
{
  File dataFile = SD.open(String(millis() / 1000), FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }
}
