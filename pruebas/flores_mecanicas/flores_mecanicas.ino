//instalar la biblioteca para #include
#include <MobaTools.h> //MobaTools permite controlar steppers de forma simultanea
#include <NewPing.h>   //trae todos los cálculos para hacer nuestras mediciones

//------------------------------------------------------
//------------ULTRASONIDOS---------------
//------------------------------------------------------
//------------VARIABLES-------------
#define NUM_SENSORES     5
#define MAX_DISTANCE     200   // distancia máxima de medición (cm)
#define UMBRAL_APERTURA  30    // distancia (cm) a la que se considera "alguien cerca"
const unsigned long PING_INTERVAL = 33; // ms mínimos entre dos mediciones del mismo sensor

// un sensor por flor, en el mismo orden que nombres_flores / pins_motor
const int pines_trigger[NUM_SENSORES] = {12, 8, 6, 4, 2};
const int pines_echo[NUM_SENSORES]    = {11, 9, 7, 5, 3};

//----------CLASSES----------
NewPing sonar[NUM_SENSORES] = {
  NewPing(pines_trigger[0], pines_echo[0], MAX_DISTANCE),
  NewPing(pines_trigger[1], pines_echo[1], MAX_DISTANCE),
  NewPing(pines_trigger[2], pines_echo[2], MAX_DISTANCE),
  NewPing(pines_trigger[3], pines_echo[3], MAX_DISTANCE),
  NewPing(pines_trigger[4], pines_echo[4], MAX_DISTANCE)
};

unsigned int distancia_cm[NUM_SENSORES];  // última lectura válida de cada sensor
unsigned long ping_timer[NUM_SENSORES];   // próximo momento (millis) en que toca medir

//------------------------------------------------------
//------------STEPPERS---------------
//------------------------------------------------------
//------------VARIABLES-------------
const int STEPS = 4096; //pasos necesarios para dar vuelta completa en HALFSTEP
int speed_steps = 200;  //revisar velocidad
const int NUM_FLORES = 5;

const char* nombres_flores[NUM_FLORES] = {"Bosque", "Desierto", "Jungla", "Nieve", "Centro"};

//PINES DE LOS MOTORES
const int pins_motor[NUM_FLORES][4] = { //ARRAY 2D---> int array[ filas ][ columnas ];
    {22, 24, 23, 25},// Pines: IN1, IN3, IN2, IN4 para cada motor
    {26, 28, 27, 29},
    {30, 32, 31, 33},
    {34, 36, 35, 37},
    {38, 40, 39, 41}
};

//----------CLASSES----------
// Array de objetos: hay que llamar al constructor de cada uno explícitamente
MoToStepper flor[NUM_FLORES] = {
  MoToStepper(STEPS), MoToStepper(STEPS), MoToStepper(STEPS), MoToStepper(STEPS), MoToStepper(STEPS)
};

bool flor_activada[NUM_FLORES]  = {false, false, false, false, false}; // ya se disparó la apertura
bool flor_reportada[NUM_FLORES] = {false, false, false, false, false}; // ya se imprimió "llegó a destino"


void setup() {
  Serial.begin(9600); //inicializamos la consola
  Serial.println(nombres_flores[0]);

  // Attach y setSpeed con loops
  for (int i = 0; i < NUM_FLORES; i++) {
    // Pines: IN1, IN3, IN2, IN4 para cada motor [ filas ][ columnas ]
    flor[i].attach(pins_motor[i][0], pins_motor[i][1], pins_motor[i][2], pins_motor[i][3]);
    flor[i].setSpeed(speed_steps);
  }

  // Escalonar el inicio de las mediciones para que no se midan dos sensores
  // al mismo tiempo (evita interferencia entre ecos)
  for (int i = 0; i < NUM_SENSORES; i++) {
    ping_timer[i] = millis() + 75 + (i * PING_INTERVAL);
  }

  // Nota: ya NO arrancamos los motores acá. Cada flor se abre cuando
  // su propio sensor detecta proximidad (ver loop()).
}

void loop() {
  // --- 1) Medición no bloqueante de los 5 sensores, uno por vez ---
  for (int i = 0; i < NUM_SENSORES; i++) {
    if (millis() >= ping_timer[i]) {
      ping_timer[i] += PING_INTERVAL * NUM_SENSORES; // próxima vez que le toca a este sensor
      unsigned int cm = sonar[i].ping_cm();
      if (cm != 0) distancia_cm[i] = cm; // 0 = sin eco / fuera de rango, se descarta
    }
  }

  // --- 2) Disparar apertura de cada flor según su sensor ---
  for (int i = 0; i < NUM_FLORES; i++) {
    if (!flor_activada[i] && distancia_cm[i] > 0 && distancia_cm[i] < UMBRAL_APERTURA) {
      flor[i].doSteps(STEPS);
      flor_activada[i] = true;
      Serial.print("flor ");
      Serial.print(nombres_flores[i]);
      Serial.println(" activada (sensor detectó proximidad)");
    }
  }

  // --- 3) Reportar cuándo cada flor llegó a destino (una sola vez) ---
  for (int i = 0; i < NUM_FLORES; i++) {
    if (flor_activada[i] && !flor_reportada[i] && flor[i].moving() == 0) {
      Serial.print("flor ");
      Serial.print(nombres_flores[i]);
      Serial.println(" llegó a destino");
      flor_reportada[i] = true;
    }
  }
}
