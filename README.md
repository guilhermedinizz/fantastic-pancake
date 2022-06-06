# fantastic-pancake

## projeto para matéria de objetos inteligentes conectados
Este projeto usa um sensor de presença, que identifica as condições de um determinado local e através do Módulo WiFi NodeMCU ESP8266 e envia o comando para ligar e desligar uma luz, de acordo com a condição apresentada, automaticamente. Sendo programado se identificar que o local tem movimentos o LED acenderá, caso não desligará, assim como essas condições são apresentadas no app MQTT Dash para um acompanhamento a distância.

![processo] (https://raw.githubusercontent.com/guilhermedinizz/fantastic-pancake/master/processo.png)

Neste repositório encontra-se toda documentação do projeto a programação está comentada facilitando o entendimento no arquivo .ino, que foi desenvolvido no Visual Studio (PlatformIO).
Para esta automação foi utilizado o módulo Wi-Fi NodeMCU ESP8266 que faz conexão com o MQTT onde são estabelecidas as configurações do Broker, e um Sensor PIR.
