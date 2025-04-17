#include <Servo.h>
#include <DHT.h>
#include <ArduinoJson.h>
 
#define TRIG_PIN 7
#define ECHO_PIN 8
#define DHTPIN 2
#define DHTTYPE DHT11
#define SERVOPIN 11
 
DHT dht(DHTPIN, DHTTYPE);
Servo servo;
bool modoAutomatico = true;
 
int duracao = 0;
 
void setup() {
  Serial.begin(9600);
  dht.begin();
  servo.attach(SERVOPIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("Sistema iniciado.");
  Serial.println("Digite 'a' para automático ou 'm' para manual.");
 
}
 
void loop() {
  if (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == 'a') {
      modoAutomatico = true;
      Serial.println("Modo automático ativado.");
    } else if (comando == 'm') {
      modoAutomatico = false;
      Serial.println("Modo manual ativado.");
    } else if (!modoAutomatico) {
      if (comando == '1') {
        servo.write(90);
        Serial.println("Servo aberto (manual).");
      } else if (comando == '0') {
        servo.write(0);
        Serial.println("Servo fechado (manual).");
 
      }
    }
  }
 
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  duracao = pulseIn(ECHO_PIN, HIGH);
 
  int dist = duracao * 0.034 / 2;
 
  StaticJsonDocument<200> doc;
  doc["temperatura"] = temperatura;
  doc["umidade"] = umidade;
  doc["distancia"] = dist;
  serializeJson(doc, Serial);
 
  Serial.println();
 
  if (modoAutomatico) {
 
    if (dist > 0 && dist < 10) {
      servo.write(90);
    } else {
      servo.write(0);
    }
 
  }
}