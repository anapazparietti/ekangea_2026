import processing.serial.*;
import processing.sound.*;

Serial miPuerto;

// Tus mismos arreglos de sonido
SoundFile[] sonidosActivar = new SoundFile[4];

SoundFile sonidoAmbiente;
SoundFile sonidoEspecial;

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
  }

  sonidoAmbiente = new SoundFile(this, "ambiente.MP3"); 
  sonidoEspecial = new SoundFile(this, "especial.mp3");

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
    
    // Convertimos el dato recibido en un número entero
    int floresActivasActual = int(stringEntrada);
    
    // VALIDACIÓN: Evitamos ruidos extraños del puerto serial
    if (floresActivasActual >= 0 && floresActivasActual <= 4) {
      
      // ─── CASO A: ALGUIEN SE ACERCA (El número de flores aumentó) ───
      if (floresActivasActual > floresActivasAnterior) {
        // Determinamos qué nivel de sonido toca activar (0, 1, 2 o 3)
        int indexSonido = floresActivasActual - 1; 
        
        if (indexSonido >= 0 && indexSonido < 4) {
          println("Se activó una flor. Nivel actual: " + floresActivasActual + " -> Play activar" + indexSonido + ".mp3");
          sonidosActivar[indexSonido].play(); // Se reproduce en paralelo, NO corta los anteriores
        }
      }
      
      // ─── CASO B: ALGUIEN SE VA (El número de flores disminuyó) ───
      else if (floresActivasActual < floresActivasAnterior) {
        // Identificamos qué capa se tiene que apagar o qué sonido de cierre poner
        int indexSonido = floresActivasActual; 
        
        if (indexSonido >= 0 && indexSonido < 4) {
          println("Se cerró una flor. Nivel actual: " + floresActivasActual + " -> Play desactivar" + indexSonido + ".mp3");
          
          // Opcional: Si querés que la capa de activación deje de sonar al bajarse, descomentá la línea de abajo:
          // sonidosActivar[indexSonido].stop(); 
          
        }
      }
      
      // Guardamos el estado para el próximo ciclo
      floresActivasAnterior = floresActivasActual;
      
      // ─── 3. LÓGICA DEL 5TO MOTOR (Clímax de la instalación) ───
      boolean motorGeneralActual = (floresActivasActual == 4);
      
      if (motorGeneralActual == true && motorGeneralAnterior == false) {
        println("¡MÁXIMO CLÍMAX! 5to Motor ON -> Play especial.mp3");
        sonidoEspecial.loop();
      }
      
      if (motorGeneralActual == false && motorGeneralAnterior == true) {
        println("El clímax terminó -> Play flor5Cierre.mp3");
        sonidoEspecial.stop();
      }
      
      motorGeneralAnterior = motorGeneralActual;
    }
  }
}
