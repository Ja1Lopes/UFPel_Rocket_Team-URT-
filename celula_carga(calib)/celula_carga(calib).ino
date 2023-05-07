#include "HX711.h"

#define DT A1
#define SCK A0

HX711 escala;

void setup()
{
  escala.begin(DT, SCK);
  Serial.begin(115200);
  Serial.print("Leitura da Tara:  ");
  Serial.println(escala.read());
  Serial.println("Aguarde!");
  Serial.println("Iniciando ...");
  escala.set_scale();
  escala.tare(20);
  Serial.println("Insira o item para Pesar");
}

/*void loop()
{
  for (int i = 0; i < 100; i++)
  {
    Serial.println(escala.get_value(10), 0);
    delay(100);
  }
  Serial.println("n");
}*/
void loop()
{
  Serial.println(escala.get_value(100), 0);
  delay(10000);
}