#include "HX711.h"

#define DT A1
#define SCK A0

const float scale = 102234.25;

HX711 scaleSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

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

  // Send the data to the PC via USB
  Serial.print(weight);
  Serial.print(",");
  Serial.println(rawValue);

  // Add a delay to control the rate of readings
  delay(50);
}
