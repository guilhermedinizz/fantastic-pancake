#include <Arduino.h>

//Incluindo as Bibliotecas da Placa NodeMCU ESP8266 para estabelecer conexão via WiFi
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int pinBruzzer = 7;
int pinSensorPIR = 8;
int pinLed = 9;
int valorSensorPIR = 0;

const char* ssid = "SSID";             //SSID da rede WIFI
const char* password =  "PASSWORD";    //senha da rede wifi

//Informações da Instância do broker MQTT
const char* mqttServer = "broker.mqtt-dashboard.com";   //server
const char* mqttUser = "user";              //user
const char* mqttPassword = "password";      //password
const int mqttPort = 1883;                     //port

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(9600);

  // Definir pinos como de entrada ou de saida
  pinMode(pinBruzzer, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinSensorPIR, INPUT);

  WiFi.begin(ssid, password);

  Serial.print("Entrando no Setup");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.println("Aguarde! Conectando ao WiFi...");
#endif
  }
#ifdef DEBUG
  Serial.println("Conectado na rede WiFi com sucesso!");
#endif

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
#ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
#endif

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
#ifdef DEBUG
      Serial.println("Conectado com sucesso");
#endif

    } else {
#ifdef DEBUG
      Serial.print("falha estado  ");
      Serial.print(client.state());
#endif
      delay(2000);
    }
  }

  //subscreve no tópico
  client.subscribe(mqttTopicSub);
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

      //Enviando Mensagem ao Broker
      sprintf(mensagem, "1"); //Definindo o valor UM como parâmetro para o Broker mostrar uma imagem "verde"
      Serial.print("Mensagem enviada: ");
      Serial.println(mensagem); 
      client.publish("casa/lampada", mensagem);
      Serial.println("Mensagem enviada com sucesso...");
  }
  else
  {
    desligarAlarme();

       //Enviando Mensagem ao Broker
      sprintf(mensagem, "0"); //Definindo o valor zero como parâmetro para o Broker mostrar uma imagem "vermelha"
      Serial.print("Mensagem enviada: ");
      Serial.println(mensagem);
      client.publish("casa/lampada", mensagem);
      Serial.println("Mensagem enviada com sucesso...");
  
  }
  delay(1000);

  client.loop();
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


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if (message == "on") { ligarAlarme }   // LED on
    if (message == "off") { desligarAlarme } // LED off
    Serial.println();
    Serial.println("-----------------------");
}

//Função pra reconectar ao servidor MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
#ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
#endif

    bool conectado = strlen(mqttUser) > 0 ?
                     client.connect("ESP8266Client", mqttUser, mqttPassword) :
                     client.connect("ESP8266Client");

    if (conectado) {
#ifdef DEBUG
      Serial.println("Conectado!");
#endif
      //Subscreve no tópico
      client.subscribe(mqttTopicSub, 1); //nivel de qualidade: QoS 1
    } else {
#ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
#endif
      //Aguarda 10 segundos
      delay(5000);
    }
  }
}