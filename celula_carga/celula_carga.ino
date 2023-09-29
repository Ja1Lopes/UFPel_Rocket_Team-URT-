#include "HX711.h"

#define DT A1
#define SCK A0

const float scale = 102234.25;

HX711 escala;


void setup()
{
  escala.begin(DT, SCK);
  Serial.begin(115200);
  Serial.print("Leitura da Tara:  ");
  Serial.println(escala.read());
  Serial.println("Aguarde!");
  Serial.println("Iniciando ...");
  escala.set_scale(scale);
  escala.tare(20);
  Serial.println("Insira o item para Pesar");
}

void loop()
{
    Serial.print("Peso em Kg: ");
    Serial.println(escala.get_units(20), 3);
}