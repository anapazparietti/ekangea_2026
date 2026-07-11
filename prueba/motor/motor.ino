#include <Stepper.h>

// Cambia esto por el número de pasos de tu motor (el 28BYJ-48 tiene 2048)
const int pasosPorRevolucion = 2048; 

// Inicializa la librería en los pines 8, 10, 9, 11 (orden específico para ULN2003)
Stepper miStepper(pasosPorRevolucion, 22, 24, 23, 25);

void setup() {
  miStepper.setSpeed(10); // Velocidad en RPM (para este motor, 10-15 es ideal)
  Serial.begin(9600);
  Serial.println("Prueba de Stepper Iniciada");
}

void loop() {
  // Girar una vuelta a la derecha
  Serial.println("Girando a la derecha...");
  miStepper.step(pasosPorRevolucion);
  delay(1000);

  // Girar una vuelta a la izquierda
  Serial.println("Girando a la izquierda...");
  miStepper.step(-pasosPorRevolucion);
  delay(1000);
}