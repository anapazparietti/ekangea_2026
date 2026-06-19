#include <NewPing.h>
#include <Stepper.h>

// -------- Ultrasonido --------
const int echoPin = 5;
const int trigPin = 6;
const int maxDistance = 200;      // cm
NewPing sonar(trigPin, echoPin, maxDistance); 

// -------- Stepper (28BYJ-48 + ULN2003 típico) --------
const int stepsPerRevolution = 2048; // 2048 para 28BYJ-48 en media reducción
//definicion de pins
const int motorPin1 = 8;    // 28BYJ48 In1
const int motorPin2 = 9;    // 28BYJ48 In2
const int motorPin3 = 10;   // 28BYJ48 In3
const int motorPin4 = 11;   // 28BYJ48 In4
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4);

// -------- Lógica de control --------
const int distanciaCerca = 10;    // mano muy cerca
const int distanciaMedia = 25;    // mano a media distancia
const int distanciaLejos = 40;    // mano lejos (pero detectada)

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(10); // RPM base (ajusta según tu motor)
}

void loop() {
  delay(60); // >29 ms recomendado entre pings

  int distancia = sonar.ping_cm(); // 0 = fuera de rango
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Si no detecta nada, no mover
  if (distancia == 0) return;

  // Control por proximidad:
  // Más cerca = más movimiento en sentido horario
  // Más lejos = sentido antihorario (opcional)
  if (distancia <= distanciaCerca) {
    myStepper.setSpeed(18);   // más rápido
    myStepper.step(40);       // avanza pasos
  } 
  else if (distancia <= distanciaMedia) {
    myStepper.setSpeed(12);
    myStepper.step(20);
  } 
  else if (distancia <= distanciaLejos) {
    myStepper.setSpeed(8);
    myStepper.step(8);
  } 
  else {
    // muy lejos: retrocede suavemente (si no quieres retroceso, comenta esta línea)
    myStepper.setSpeed(6);
    myStepper.step(-10);
  }
}