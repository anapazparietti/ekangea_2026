//instalar la biblioteca para #include
#include <MobaTools.h> 
//MobaTools permite controlar steppers de forma simultanea

//------------VARIABLES-------------
const int STEPS = 4096; //pasos necesarios para dar vuelta completa en HALFSTEP
int SPEED_STEPS = 200; //revisar velocidad
const int NUM_FLORES = 5;
char Flor[NUM_FLORES] = {"Bosque", "Desierto", "Jungla", "Nieve", "Centro"};



//----------CLASSES----------
MoToStepper Flor[0](STEPS); //HALFSTEP está por defecto ---> MoToStepper Name(STEPS,SECUENCIA);
MoToStepper Flor[1](STEPS);
MoToStepper Flor[2](STEPS); 
MoToStepper Flor[3](STEPS);
MoToStepper Flor[4](STEPS);

// Pines: IN1, IN3, IN2, IN4 para cada motor
const int pins[NUM_FLORES][4] = { //ARRAY 2D---> int array[ filas ][ columnas ];
    {22, 24, 23, 25},
    {26, 28, 27, 29},
    {30, 32, 31, 33},
    {34, 36, 35, 37},
    {38, 40, 39, 41}
};

void setup(){
Serial.begin(9600); //inicializamos la consola(?)
Serial.println(Flor[0]); 

// Attach y setSpeed con loops
    for (int i = 0; i < NUM_FLORES; i++) {
        Flor[i].attach(pins[i][0], pins[i][1], pins[i][2], pins[i][3]);
        Flor[i].setSpeed(SPEED_STEPS);
    };

// Iniciar todos
    for (int i = 0; i < NUM_FLORES; i++) {
        Flor[i].doSteps(STEPS);
    };

};

void loop(){
//las flores se mueven en cadena
for (int i = 0; i < NUM_STEPPERS; i++) {
  if (Flor[i].moving() == 0) {
    Serial.print("Flor ");
    Serial.print(Flor[i]);
    Serial.println(" llegó a destino");
    };
  };

};