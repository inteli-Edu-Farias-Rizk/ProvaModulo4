#include <WiFi.h>
#include <HTTPClient.h>

#define led_azul 9 // Pino utilizado para controle do led azul
#define led_verde 41 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int pinobotao = 18;  // O numero do pino do botão
int EstadoBotao = 0;  // Variavel para ler o status do botão

const int ldrPin = 4;  // O numero do pino do ldr 
int threshold=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_azul,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(pinobotao, INPUT); // Inicializa o botão pin como input

  digitalWrite(led_azul, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  EstadoBotao = digitalRead(pinobotao);
  if (EstadoBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){
    Serial.print("Está escuro ligue o led");
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
    delay(1000);
    Serial.println(ldrstatus);

  }else{
    Serial.print("Está claro desligue a luz");

    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);


    
    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);

    digitalWrite(led_vermelho, HIGH);

    if(digitalRead(pinobotao) == HIGH){
      EstadoBotao = 1;
    }else{
      EstadoBotao = 0;
    }


    if(EstadoBotao == 1){
      delay(1000);
      digitalWrite(led_vermelho, LOW);
    }else{
      delay(5000);
      digitalWrite(led_vermelho, LOW);
    }

   




    Serial.println(ldrstatus);
  }
}