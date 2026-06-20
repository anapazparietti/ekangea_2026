//instalar la biblioteca para #include
#include <MobaTools.h> //MobaTools permite controlar steppers de forma simultanea
#include <NewPing.h> //trae todos los cálculos para hacer nuestras mediciones

//------------------------------------------------------
//------------ULTRASONIDOS---------------
//------------------------------------------------------
//------------VARIABLES-------------
int max_distance = 200;
const int NUM_SONAR = 4;
const char* nombre_sonar[NUM_SONAR] = {"bosque", "desierto", "jungla", "nieve"};
 
//----------CLASSES----------
NewPing sonar[NUM_SONAR] = {
  //TRIGGER_PIN, ECHO_PIN
  NewPing(42, 43, max_distance), // sonar bosque ---> nombre_sonar[0] ---> sonar[0]
  NewPing(48, 49, max_distance), // sonar desierto ---> sonar[1]
  NewPing(50, 51, max_distance), // sonar jungla ---> sonar[2]
  NewPing(52, 53, max_distance) // sonar nieve ---> sonar[3]
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
for (int i = 0; i < NUM_STEPPERS; i++) {
  if (flor[i].moving() == 0) {
    Serial.print("flor ");
    Serial.print(nombre_flor[i]);
    Serial.println(" se abrió");
    };
  };

};