#include <AccelStepper.h>
#include <NewPing.h>

//STEPPERS---------------------------
const int NUM_ZONAS = 4;

// Definimos la velocidad constante de giro continuo (pasos por segundo)
// Positivo gira a un lado, negativo al otro (ej: -600)
const int VELOCIDAD_CONTINUA = 600; 

const int pines_motor[NUM_ZONAS][4] = {
  {22, 23, 24, 25}, // Bosque
  {26, 27, 28, 29}, // Desierto
  {30, 31, 32, 33}, // Jungla
  {34, 35, 36, 37}  // Nieve
};

AccelStepper motores[NUM_ZONAS] = {
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[0][0], pines_motor[0][2], pines_motor[0][1], pines_motor[0][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[1][0], pines_motor[1][2], pines_motor[1][1], pines_motor[1][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[2][0], pines_motor[2][2], pines_motor[2][1], pines_motor[2][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[3][0], pines_motor[3][2], pines_motor[3][1], pines_motor[3][3])
};

// QUINTO STEPPER GENERAL ------------------
AccelStepper motor_general(AccelStepper::FULL4WIRE, 50, 52, 51, 53);


//ULTRASONIDOS--------------------
const int MaxDistance = 200;
const int DISTANCIA_ACTIVACION = 10; 

const int pines_sonar[NUM_ZONAS][2] = {
  {43, 42}, // Bosque
  {45, 44}, // Desierto
  {47, 46}, // Jungla
  {49, 48}  // Nieve
};

NewPing sonares[NUM_ZONAS] = {
  NewPing(pines_sonar[0][0], pines_sonar[0][1], MaxDistance),
  NewPing(pines_sonar[1][0], pines_sonar[1][1], MaxDistance),
  NewPing(pines_sonar[2][0], pines_sonar[2][1], MaxDistance),
  NewPing(pines_sonar[3][0], pines_sonar[3][1], MaxDistance)
};

int distancias[NUM_ZONAS] = {0, 0, 0, 0};
unsigned long tiempo_sensores = 0;
const int INTERVALO_SENSORES = 120; 


//VOID SET UP--------------------------------
void setup() {
  Serial.begin(9600);

  // Para giro continuo con runSpeed(), solo necesitamos configurar setMaxSpeed
  for (int i = 0; i < NUM_ZONAS; i++) {
    motores[i].setMaxSpeed(1000);
  }
  motor_general.setMaxSpeed(1000);
}


//VOID LOOP----------------------------------
void loop() {

  // LECTURA DE SENSORES 
  if (millis() - tiempo_sensores >= INTERVALO_SENSORES) {
    tiempo_sensores = millis();
    for (int i = 0; i < NUM_ZONAS; i++) {
      distancias[i] = sonares[i].ping_cm();
    }
  }

  bool los_4_sensores_detectan = true;

  // LÓGICA DE LOS 4 STEPPERS INDIVIDUALES
  for (int i = 0; i < NUM_ZONAS; i++) {
    
    if (distancias[i] > 0 && distancias[i] <= DISTANCIA_ACTIVACION) {
      // Si detecta, le asignamos velocidad constante para que avance sin parar
      motores[i].setSpeed(VELOCIDAD_CONTINUA);
      motores[i].runSpeed();
    } else {
      // Si no detecta, velocidad a 0 y frena de golpe
      motores[i].setSpeed(0);
      los_4_sensores_detectan = false;
    }
  }


  // LÓGICA DEL QUINTO STEPPER GENERAL -----------------------
  if (los_4_sensores_detectan) {
    // Si los 4 están activos, gira continuamente
    motor_general.setSpeed(VELOCIDAD_CONTINUA);
    motor_general.runSpeed();
  } else {
    // Si falta uno solo, se detiene
    motor_general.setSpeed(0);
  }
}