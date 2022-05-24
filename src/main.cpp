#include <Arduino.h>

int pinBruzzer = 7;
int pinSensorPIR = 8;
int pinLed = 9;
int valorSensorPIR = 0;

void setup()
{
  Serial.begin(9600);

  // Definir pinos como de entrada ou de saida
  pinMode(pinBruzzer, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinSensorPIR, INPUT);
}

void loop()
{
  // Lendo o valor do sensor PIR.
  // O sensor pode assumir 2 valores: 0 e 1
  // 1 quando detecta algum movimento
  // 0 quando não detecta movimento.
  valorSensorPIR = digitalRead(pinSensorPIR);

  Serial.print("Valor do Sensor PIR : ");
  Serial.print(valorSensorPIR);

  // Verifica se ocorreu detecção de movimento
  if (valorSensorPIR == 1)
  {
    ligarAlarme();
  }
  else
  {
    desligarAlarme();
  }
}

void ligarAlarme()
{
  // Ligando o led
  digitalWrite(pinLed, HIGH);

  // Ligando o buzzer com uma frequencia de 2000hz
  tone(pinBruzzer, 2000);

  delay(2000); // tempo que o led fica acesso e o buzzer toca

  desligarAlarme();
}

void desligarAlarme()
{
  // Desliga o led
  digitalWrite(pinLed, LOW);
}