#include <AccelStepper.h>
#include <NewPing.h>

//STEPPERS---------------------------
const int NUM_ZONAS = 4; 
const int VELOCIDAD_CONTINUA = 600; 

const int pines_motor[NUM_ZONAS][4] = {
  {22, 23, 24, 25}, // Índice 0: Bosque
  {26, 27, 28, 29}, // Índice 1: Desierto
  {30, 31, 32, 33}, // Índice 2: Jungla
  {34, 35, 36, 37}  // Índice 3: Nieve
};

// Array de textos para identificar las filas de la matriz en los prints
const String nombres_zonas[NUM_ZONAS] = {"BOSQUE", "DESIERTO", "JUNGLA", "NIEVE"};

AccelStepper motores[NUM_ZONAS] = {
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[0][0], pines_motor[0][2], pines_motor[0][1], pines_motor[0][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[1][0], pines_motor[1][2], pines_motor[1][1], pines_motor[1][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[2][0], pines_motor[2][2], pines_motor[2][1], pines_motor[2][3]),
  AccelStepper(AccelStepper::FULL4WIRE, pines_motor[3][0], pines_motor[3][2], pines_motor[3][1], pines_motor[3][3])
};

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
bool estado_flores[NUM_ZONAS] = {false, false, false, false}; 
bool estado_flores_anterior[NUM_ZONAS] = {false, false, false, false}; // Para detectar cambios físicos

// TIMERS
unsigned long tiempo_sensores = 0;
const int INTERVALO_SENSORES = 120; 

unsigned long tiempo_processing = 0;
const int INTERVALO_PROCESSING = 150; 


void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < NUM_ZONAS; i++) {
    motores[i].setMaxSpeed(1000);
  }
  motor_general.setMaxSpeed(1000);
}


void loop() {

  // 1. LECTURA DE SENSORES
  if (millis() - tiempo_sensores >= INTERVALO_SENSORES) {
    tiempo_sensores = millis();
    for (int i = 0; i < NUM_ZONAS; i++) {
      distancias[i] = sonares[i].ping_cm();
    }
  }

  // 2. LÓGICA DE VELOCIDADES, ESTADOS Y MONITOREO
  bool los_4_sensores_detectan = true;

  for (int i = 0; i < NUM_ZONAS; i++) {
    if (distancias[i] > 0 && distancias[i] <= DISTANCIA_ACTIVACION) {
      motores[i].setSpeed(VELOCIDAD_CONTINUA);
      estado_flores[i] = true; 
    } else {
      motores[i].setSpeed(0);
      estado_flores[i] = false; 
      los_4_sensores_detectan = false;
    }

    // ─── MONITOREO EN CONSOLA ARDUINO ───
    // Detecta el flanco exacto en el que cambia el estado de la fila evaluada
    if (estado_flores[i] == true && estado_flores_anterior[i] == false) {
      // Al anteponer un texto, Processing ignorará esta línea al intentar pasarla a número (int), 
      // pero a vos te sirve para leerlo en el Monitor de Arduino.
      Serial.println("-> [ACTUALIZACION] Flor abierta en zona: " + nombres_zonas[i]);
    }
    if (estado_flores[i] == false && estado_flores_anterior[i] == true) {
      Serial.println("-> [ACTUALIZACION] Flor cerrada en zona: " + nombres_zonas[i]);
    }
    
    // Guardamos el histórico local de la fila
    estado_flores_anterior[i] = estado_flores[i];
  }

  // Motor general
  if (los_4_sensores_detectan) {
    motor_general.setSpeed(VELOCIDAD_CONTINUA);
  } else {
    motor_general.setSpeed(0);
  }

  // 3. ENVIAR TOTAL DE FLORES ACTIVAS A PROCESSING
  if (millis() - tiempo_processing >= INTERVALO_PROCESSING) {
    tiempo_processing = millis();
    
    int conteo_total = 0;
    for (int i = 0; i < NUM_ZONAS; i++) {
      if (estado_flores[i] == true) {
        conteo_total++;
      }
    }
    
    // Enviamos el número limpio para Processing
    Serial.println(conteo_total); 
  }

  // 4. EJECUCIÓN DE MOTORES
  for (int i = 0; i < NUM_ZONAS; i++) {
    motores[i].runSpeed();
  }
  motor_general.runSpeed();
}