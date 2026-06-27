import processing.serial.*;
import processing.sound.*;

Serial miPuerto;

// Tus mismos arreglos de sonido
SoundFile[] sonidosActivar = new SoundFile[4];
SoundFile[] sonidosDesactivar = new SoundFile[4];

SoundFile sonidoAmbiente;
SoundFile sonidoEspecial;
SoundFile flor5Cierre;

// REGISTRO DE ESTADO
int floresActivasAnterior = 0; 
boolean motorGeneralAnterior = false;

void setup() {
  size(400, 200);
  
  String portName = Serial.list()[0]; // Recordá cambiar el índice si es necesario
  miPuerto = new Serial(this, portName, 9600);
  miPuerto.bufferUntil('\n'); 

  // Carga de sonidos (se mantiene igual)
  for (int i = 0; i < 4; i++) {
    sonidosActivar[i] = new SoundFile(this, "activar" + i + ".mp3");
    sonidosDesactivar[i] = new SoundFile(this, "desactivar" + i + ".mp3");
  }

  sonidoAmbiente = new SoundFile(this, "ambiente.MP3"); 
  sonidoEspecial = new SoundFile(this, "especial.mp3");
  flor5Cierre    = new SoundFile(this, "flor5Cierre.mp3");

  sonidoAmbiente.loop();
}

void draw() {
  background(30);
  fill(255);
  text("Conteo dinámico de flores: " + floresActivasAnterior, 20, 50);
}


void serialEvent(Serial puerto) {
  String stringEntrada = puerto.readStringUntil('\n');
  
  if (stringEntrada != null) {
    stringEntrada = trim(stringEntrada);
    
    // ─── MOSTRAR EN LA CONSOLA DE PROCESSING ───
    // Esto hace que veas los carteles de "BOSQUE", "DESIERTO", etc., directo acá abajo.
    println(stringEntrada); 
    
    // Intentamos convertir a número
    int floresActivasActual = int(stringEntrada);
    
    // VALIDACIÓN: La lógica musical solo avanza si el dato es un número válido (0 al 4)
    if (floresActivasActual >= 0 && floresActivasActual <= 4) {
      
      // CASO A: ALGUIEN SE ACERCA
      if (floresActivasActual > floresActivasAnterior) {
        int indexSonido = floresActivasActual - 1; 
        if (indexSonido >= 0 && indexSonido < 4) {
          sonidosActivar[indexSonido].play();
        }
      }
      
      // CASO B: ALGUIEN SE VA
      else if (floresActivasActual < floresActivasAnterior) {
        int indexSonido = floresActivasActual; 
        if (indexSonido >= 0 && indexSonido < 4) {
          sonidosDesactivar[indexSonido].play();
        }
      }
      
      floresActivasAnterior = floresActivasActual;
      
      // LÓGICA DEL 5TO MOTOR
      boolean motorGeneralActual = (floresActivasActual == 4);
      if (motorGeneralActual == true && motorGeneralAnterior == false) {
        flor5Cierre.stop();
        sonidoEspecial.loop();
      }
      if (motorGeneralActual == false && motorGeneralAnterior == true) {
        sonidoEspecial.stop();
        flor5Cierre.play();
      }
      motorGeneralAnterior = motorGeneralActual;
    }
  }
}
