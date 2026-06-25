//instalar la biblioteca para #include
#include <MobaTools.h> //MobaTools permite controlar steppers de forma simultanea
#include <NewPing.h> //trae todos los cálculos para hacer nuestras mediciones

//------------------------------------------------------
//------------ULTRASONIDOS---------------
//------------------------------------------------------
//------------VARIABLES-------------
#define PING_INTERVAL 33 // Milesegundos entre pings (sonido/señal de disparode un mismo sensor (>29ms para evitar "cross-talk").
#define NUM_SONAR 4
#define MAX_DISTANCIA 200

const char* nombre_sonar[NUM_SONAR] = {"bosque", "desierto", "jungla", "nieve"};
unsigned long pingTimer[NUM_SONAR]; // Holds the times when the next ping should happen for each sensor
unsigned int cm[NUM_SONAR];         // Where the ping distances are stored.
uint8_t current_sonar = 0;          // Keeps track of which sensor is active. uint8_7 es un tipo de unsigned int de una extensión de 8 bits

//----------CLASSES----------
NewPing sonar[NUM_SONAR] = {
  //TRIGGER_PIN, ECHO_PIN
  NewPing(42, 43, MAX_DISTANCIA), // sonar bosque ---> nombre_sonar[0] ---> sonar[0]
  NewPing(48, 49, MAX_DISTANCIA), // sonar desierto ---> sonar[1]
  NewPing(50, 51, MAX_DISTANCIA), // sonar jungla ---> sonar[2]
  NewPing(52, 53, MAX_DISTANCIA) // sonar nieve ---> sonar[3]
};


//------------------------------------------------------
//------------STEPPERS---------------
//------------------------------------------------------
//------------VARIABLES-------------
const int STEPS = 4096; //pasos necesarios para dar vuelta completa en HALFSTEP
int speed_steps = 200; //revisar velocidad
const int NUM_FLORES = 5;
//PINES DE LOS MOTORES
const int pins_motor[NUM_FLORES][4] = { //ARRAY 2D---> int array[ filas ][ columnas ];
    // Pines: IN1, IN3, IN2, IN4
    {22, 24, 23, 25},// flor bosque ---> nombre_flor[0] ---> flor[0] 
    {26, 28, 27, 29},// flor desierto ---> flor[1]
    {30, 32, 31, 33},// flor jungla ---> flor[2]
    {34, 36, 35, 37},// flor nieve ---> flor[3]
    {38, 40, 39, 41}//  flor centro ---> flor[4]
};
const char* nombre_flor[NUM_FLORES] = {"bosque", "desierto", "jungla", "nieve", "centro"};
//----------CLASSES----------
MoToStepper flor[NUM_FLORES] = {
  // cada clase que se declara corresponde a una flor
  MoToStepper(STEPS), 
  MoToStepper(STEPS),
  MoToStepper(STEPS),
  MoToStepper(STEPS),
  MoToStepper(STEPS)
};

void setup(){
Serial.begin(9600); //inicializamos la consola

pingTimer[0] = millis() + 75; //El sensor 0 dispara por primera vez 75 ms después de arrancar (para que el Arduino se estabilice).
for (uint8_t i = 1; i < SONAR_NUM; i++){
  pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL; //hace la cuenta i - 1 para no salirse del arreglo
};

// Attach y setSpeed con loops
    for (int i = 0; i < NUM_FLORES; i++) {
        //.attach es un metodo de la clase MoToStepper, vincula cada objeto flor con sus respectivos pines
        //Mantener concidencia entre array2D y array nombre_flores
        // Pines: IN1, IN3, IN2, IN4 para cada motor [ filas ][ columnas ]
        flor[i].attach(pins_motor[i][0], pins_motor[i][1], pins_motor[i][2], pins_motor[i][3]); 
        flor[i].setSpeed(speed_steps);
    };

// Iniciar todos
    for (int i = 0; i < NUM_FLORES; i++) {
        flor[i].doSteps(STEPS);
    };

};

void loop(){
//las flores se mueven en cadena
for (int i = 0; i < NUM_FLORES; i++) {
  if (flor[i].moving() == 0) {
    Serial.print("flor ");
    Serial.print(nombre_flor[i]);
    Serial.println(" se abrió");
    };
  };

};