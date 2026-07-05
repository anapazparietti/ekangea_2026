import processing.serial.*;
import processing.sound.*;

Serial miPuerto;

SoundFile[] sonidoActivar = new SoundFile[4];
SoundFile sonidoAmbiente;

int contenidoActual = 0;

boolean keyz[] = new boolean [4];


void setup() {
  size(1000, 750, P2D);
  smooth(4);

  /*
  //puerto de comunicación
   printArray(Serial.list());
   String portName = Serial.list()[0];
   miPuerto = new Serial(this, portName, 9600);
   miPuerto.bufferUntil('\n');//delimitador del flujo interrumpido de datos (bytes) que envía Arduino
   */

  for (int i = 0; i < sonidoActivar.length; i++) {
    sonidoActivar[i] = new SoundFile(this, "activar" + i + ".mp3");
  };
  sonidoAmbiente = new SoundFile(this, "ambiente.MP3");
  sonidoAmbiente.loop();

  /*
  println(sonidoActivar.length);
   println();
   */
};

void draw() {
  /*
  //teclas que son presionadas
   printArray(keyz);
   println();
   */

  background(10);

  textSize(128);
  text(contenidoActual, 40, 120);

  //EVENTOS POR TECLADO
  controlTeclado();
};

void serialEvent(Serial p) {
  String stringEntrada = p.readStringUntil('\n');
  //extrae del búfer todo el contenido recibido hasta el salto de línea, devuelve String
  if (stringEntrada != null) {
    //si por alguna razón la lectura no captura una línea completa, la función devuelve null, y el bloque de código posterior no se ejecuta, evitando errores.
    stringEntrada = trim(stringEntrada);
    //trim() elimina espacios en blanco
    // Convertimos el dato recibido en un número entero
    int floresActivas = int(stringEntrada); //actua como la variable posicion
    //se convierte la cadenada en int
    if (floresActivas >= 0  && floresActivas <= 4) {
      //los valores invalidos (líneas de monitoreo) son descartados
      if (floresActivas > contenidoActual) {
        //contenidoActual en principio es 0, por lo que valores = o < a 0 no ingresan en la condición
        int indice = floresActivas - 1;
        if (indice >= 0 && indice < sonidoActivar.length) {
          if (!sonidoActivar[indice].isPlaying()) {
            sonidoActivar[indice].play();
          };
        };
        //detener sonido anterior
        for (int w = 0; w < indice; w++) {
          if (sonidoActivar[w].isPlaying() || sonidoActivar[indice].isPlaying()) {
            sonidoActivar[w].stop();
          };
        };
      };
    };
    contenidoActual = floresActivas;
    println("hay" + floresActivas + "flores activas");
  };
};

void controlTeclado() {
  int posicion = 0; // Variable de conteo, independiente del índice de la tecla
  for (int i = 0; i < keyz.length; i++) {
    if (keyz[i]) {
      rect(posicion*100, width/2, 100, 50); // Se utiliza "posicion", no "i"
      posicion++; // Se incrementa solo cuando se dibuja un cuadrado
    };
  };
  text(posicion, 100, 120);

  //ESTADOS POR SONIDO (pensar en una caja, la cual va cambiando su contenido)
  if (posicion > contenidoActual) {
    int indice = posicion - 1; //no usar posición o contenidoActual, se pasa del limite del array
    println();
    println("indice es = " + indice);
    println();
    if (indice >= 0 && indice < sonidoActivar.length) {
      if (!sonidoActivar[indice].isPlaying()) {//para que no se repita
        sonidoActivar[indice].play();
      };
    };
    //detener sonido anterior
    for (int j = 0; j < indice; j++) {
      println("j es = " + j);
      if (sonidoActivar[j].isPlaying() || sonidoActivar[indice].isPlaying()) {
        sonidoActivar[j].stop();
      };
    };
  };
  contenidoActual = posicion;
};


void keyPressed() {

  if (key == 'a')  keyz[0] = true;
  if (key == 's')  keyz[1] = true;
  if (key == 'd')  keyz[2] = true;
  if (key == 'f')  keyz[3] = true;
};

void keyReleased() {

  if (key == 'a')  keyz[0] = false;
  if (key == 's')  keyz[1] = false;
  if (key == 'd')  keyz[2] = false;
  if (key == 'f')  keyz[3] = false;
};

/*
void serialEvent(Serial miPuerto){
 
 };
 */
