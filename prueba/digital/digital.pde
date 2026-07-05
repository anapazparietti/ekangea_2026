import processing.serial.*;
import processing.sound.*;

Serial miPuerto;

SoundFile[] sonidosActivar = new SoundFile[4];
SoundFile sonidoAmbiente;

int contenidoActual = 0;

boolean keyz[] = new boolean [4];


void setup() {
  size(1000, 750);
  // String portName = Serial.list()[0];
  // miPuerto = new Serial(this, portName, 9600);
  // miPuerto.bufferUntil('\n');//delimitador del flujo interrumpido de datos (bytes) que envía Arduino
  for (int i = 0; i < sonidosActivar.length; i++) {
    sonidosActivar[i] = new SoundFile(this, "activar" + i + ".mp3");
  };
  sonidoAmbiente = new SoundFile(this, "ambiente.MP3");
  sonidoAmbiente.loop();
  /*
  println(sonidosActivar.length);
   println();
   */
};

void draw() {

  //teclas que son presionadas
  printArray(keyz);
  println();


  background(30);


  textSize(128);
  text(contenidoActual, 40, 120);


  int posicion = 0; // Variable de conteo, independiente del índice de la tecla


  for (int i = 0; i < keyz.length; i++) {
    if (keyz[i]) {
      rect(posicion*100, width/2, 100, 50); // Se utiliza "posicion", no "i"
      posicion++; // Se incrementa solo cuando se dibuja un cuadrado
    };
  };
  text(posicion, 100, 120);


  if (posicion > contenidoActual) {
    int indice = posicion - 1;
    if (indice >= 0 && indice < sonidosActivar.length) {
      if (!sonidosActivar[indice].isPlaying()) {
        sonidosActivar[indice].play();
      };
    };

    for (int j = 0; j < indice; j++) {
      if (sonidosActivar[j].isPlaying()) {
        sonidosActivar[j].stop();
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
 
  /*
  if(sonidosActivar[contenidoActual].isPlaying()){
    sonidosActivar[contenidoActual].stop();
  };
*/



};
