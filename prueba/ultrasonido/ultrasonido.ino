#include <NewPing.h>

#define TRIGGER_PIN  42  // Pin del Trigger
#define ECHO_PIN     43  // Pin del Echo
#define MAX_DISTANCE 400 // Distancia máxima en cm

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(50); // Espera breve entre mediciones
  unsigned int cm = sonar.ping_cm(); // Obtiene la distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(cm);
  Serial.println(" cm");
}