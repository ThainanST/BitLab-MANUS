#include <Servo.h>

#define SERVO_PIN 9
#define PULSE_MIN 500
#define PULSE_MAX 2500

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.write(0);
  myServo.attach(SERVO_PIN, PULSE_MIN, PULSE_MAX);
  Serial.println("--- Interface Serial de Controle de Servo ---");
  Serial.println("Digite um angulo entre 0 e 180 e pressione ENTER:");
}

void loop() {
  if (Serial.available() > 0) {
    int anguloAlvo = Serial.parseInt();
    if (anguloAlvo >= 0 && anguloAlvo <= 180) {
      Serial.print("Movendo para: ");
      Serial.print(anguloAlvo);
      Serial.println(" graus.");
      myServo.write(anguloAlvo);
    } else {
      Serial.println("Erro: Digite um valor entre 0 e 180.");
    }
    while(Serial.available() > 0) Serial.read();
  }
}











