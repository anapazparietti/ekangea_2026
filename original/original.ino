#include <AccelStepper.h>
#include <Servo.h>
#include <NewPing.h>

//STEPPERS---------------------------
const int NUM_ZONAS = 4;
int pasos_destino = 2048; 

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

unsigned long tiempos_espera[NUM_ZONAS] = {0, 0, 0, 0};
bool esperando[NUM_ZONAS] = {false, false, false, false};


//ULTRASONIDOS--------------------
const int MaxDistance = 200;
const int DISTANCIA_ACTIVACION = 30; 

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


//SERVO-MOTOR-------------------
Servo myservo;  
int angulo_servo = 0; 


//VOID SET UP--------------------------------
void setup() {
  Serial.begin(9600);

  myservo.attach(10);  
  myservo.write(angulo_servo); 

  for (int i = 0; i < NUM_ZONAS; i++) {
    motores[i].setMaxSpeed(800);
    motores[i].setAcceleration(300);
    motores[i].setCurrentPosition(0);
  }
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

  // Bandera para el servo basada ESTRICTAMENTE en los sensores
  bool los_4_sensores_detectan = true;

  //STEPPERS-----------------------
  for (int i = 0; i < NUM_ZONAS; i++) {
    
    // Evaluamos si el sensor detecta algo
    if (distancias[i] > 0 && distancias[i] <= DISTANCIA_ACTIVACION) {
      
      // Movimiento normal del stepper
      if (motores[i].distanceToGo() == 0 && !esperando[i]) {
        tiempos_espera[i] = millis();
        esperando[i] = true;
      }
      if (esperando[i] && (millis() - tiempos_espera[i] >= 1000)) {
        esperando[i] = false;
        if (motores[i].currentPosition() == pasos_destino) {
          motores[i].moveTo(0);
        } else {
          motores[i].moveTo(pasos_destino);
        }
      }
    } else {
      // Si el sensor no detecta, este motor se frena
      motores[i].stop(); 
      esperando[i] = false; 
      
      // Como este sensor no detecta, rompemos la condición para el servo
      los_4_sensores_detectan = false;
    }
    
    motores[i].run();
  }


  //SERVO---------------------------------
  // El servo responde directamente a la lectura limpia de los 4 sensores
  if (los_4_sensores_detectan) {
    if (angulo_servo != 180) {
      angulo_servo = 180;
      myservo.write(angulo_servo);
    }
  } else {
    if (angulo_servo != 0) {
      angulo_servo = 0;
      myservo.write(angulo_servo);
    }
  }
}