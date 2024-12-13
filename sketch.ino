#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2       // Pino do LED verde
#define led_vermelho 40   // Pino do LED vermelho
#define led_amarelo 9     // Pino do LED amarelo

const int pinobotao = 18;  // Pino do botão
const int ldrPin = 4;      // Pino do LDR
int threshold = 600;       // Limite para leitura do LDR

void setup() {
  // Configuração dos LEDs como saída
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_amarelo, OUTPUT);

  // Configuração do botão como entrada com pull-up interno
  pinMode(pinobotao, INPUT_PULLUP);

  // Inicializa os LEDs desligados
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  // Inicialização da comunicação serial
  Serial.begin(115200);

  // Conexão ao Wi-Fi
  WiFi.begin("Wokwi-GUEST", ""); // Rede Wi-Fi aberta

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi com sucesso!");
}

void loop() {
  // Leitura do sensor LDR
  int ldrstatus = analogRead(ldrPin);

  // Controle baseado na leitura do LDR
  if (ldrstatus <= threshold) {
    Serial.println("Está escuro, ligando o LED amarelo");
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
  } else {
    Serial.println("Está claro, controlando LEDs");

    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);

    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);

    digitalWrite(led_vermelho, HIGH);

    // Controle do LED vermelho com base no botão
    if (digitalRead(pinobotao) == LOW) {
      delay(1000);
      digitalWrite(led_vermelho, LOW);
    } else {
      delay(5000);
      digitalWrite(led_vermelho, LOW);
    }
  }

  // Exibição do status do LDR
  Serial.println(ldrstatus);

  // Verifica a conexão Wi-Fi periodicamente
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://www.google.com.br/");
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro na conexão HTTP. Código: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }

  delay(10000); // Atraso para evitar requisições muito frequentes
}
