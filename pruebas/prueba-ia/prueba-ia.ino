// ---------- SENSORES ----------

#define TRIG1 22
#define ECHO1 23

#define TRIG2 24
#define ECHO2 25

#define TRIG3 26
#define ECHO3 27

#define TRIG4 28
#define ECHO4 29

// ---------- MOTORES ----------

#define STEP1 30
#define DIR1 31

#define STEP2 32
#define DIR2 33

#define STEP3 34
#define DIR3 35

#define STEP4 36
#define DIR4 37

#define STEP5 38
#define DIR5 39

// ---------- VARIABLES ----------

bool flor1Abierta = false;
bool flor2Abierta = false;
bool flor3Abierta = false;
bool flor4Abierta = false;
bool florCentralAbierta = false;

// Cantidad de pasos para abrir/cerrar
const int PASOS_APERTURA = 400;

// Distancia de activación
const int DISTANCIA_ACTIVACION = 20;

// ---------- FUNCIONES ----------

long medirDistancia(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 30000);

  return duracion * 0.034 / 2;
}

void abrirFlor(int stepPin, int dirPin) {

  digitalWrite(dirPin, HIGH);

  for (int i = 0; i < PASOS_APERTURA; i++) {

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

void cerrarFlor(int stepPin, int dirPin) {

  digitalWrite(dirPin, LOW);

  for (int i = 0; i < PASOS_APERTURA; i++) {

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);

    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

// ---------- SETUP ----------

void setup() {

  Serial.begin(9600);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);

  pinMode(TRIG4, OUTPUT);
  pinMode(ECHO4, INPUT);
//CÓDIGO HECHO CON IA PARA PRUEBAS
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);

  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);

  pinMode(STEP3, OUTPUT);
  pinMode(DIR3, OUTPUT);

  pinMode(STEP4, OUTPUT);
  pinMode(DIR4, OUTPUT);

  pinMode(STEP5, OUTPUT);
  pinMode(DIR5, OUTPUT);
}

// ---------- LOOP ----------

void loop() {

  long d1 = medirDistancia(TRIG1, ECHO1);
  long d2 = medirDistancia(TRIG2, ECHO2);
  long d3 = medirDistancia(TRIG3, ECHO3);
  long d4 = medirDistancia(TRIG4, ECHO4);

  // FLOR 1

  if (d1 < DISTANCIA_ACTIVACION && !flor1Abierta) {

    abrirFlor(STEP1, DIR1);
    flor1Abierta = true;

    Serial.println("Flor 1 abierta");
  }

  if (d1 >= DISTANCIA_ACTIVACION && flor1Abierta) {

    cerrarFlor(STEP1, DIR1);
    flor1Abierta = false;

    Serial.println("Flor 1 cerrada");
  }

  // FLOR 2

  if (d2 < DISTANCIA_ACTIVACION && !flor2Abierta) {

    abrirFlor(STEP2, DIR2);
    flor2Abierta = true;

    Serial.println("Flor 2 abierta");
  }

  if (d2 >= DISTANCIA_ACTIVACION && flor2Abierta) {

    cerrarFlor(STEP2, DIR2);
    flor2Abierta = false;

    Serial.println("Flor 2 cerrada");
  }

  // FLOR 3

  if (d3 < DISTANCIA_ACTIVACION && !flor3Abierta) {

    abrirFlor(STEP3, DIR3);
    flor3Abierta = true;

    Serial.println("Flor 3 abierta");
  }

  if (d3 >= DISTANCIA_ACTIVACION && flor3Abierta) {

    cerrarFlor(STEP3, DIR3);
    flor3Abierta = false;

    Serial.println("Flor 3 cerrada");
  }

  // FLOR 4

  if (d4 < DISTANCIA_ACTIVACION && !flor4Abierta) {

    abrirFlor(STEP4, DIR4);
    flor4Abierta = true;

    Serial.println("Flor 4 abierta");
  }

  if (d4 >= DISTANCIA_ACTIVACION && flor4Abierta) {

    cerrarFlor(STEP4, DIR4);
    flor4Abierta = false;

    Serial.println("Flor 4 cerrada");
  }

  // FLOR CENTRAL

  bool todasAbiertas =
      flor1Abierta &&
      flor2Abierta &&
      flor3Abierta &&
      flor4Abierta;

  if (todasAbiertas && !florCentralAbierta) {

    abrirFlor(STEP5, DIR5);
    florCentralAbierta = true;

    Serial.println("FLOR CENTRAL ABIERTA");
  }

  if (!todasAbiertas && florCentralAbierta) {

    cerrarFlor(STEP5, DIR5);
    florCentralAbierta = false;

    Serial.println("FLOR CENTRAL CERRADA");
  }

  delay(100);
}