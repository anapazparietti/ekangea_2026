import processing.serial.*;
import processing.sound.*;

Serial miPuerto;

// Sonidos
SoundFile sonidoAmbiente;
SoundFile[] sonidosActivar = new SoundFile[5];    // Índice 0 a 4
SoundFile[] sonidosDesactivar = new SoundFile[5]; // Índice 0 a 4
SoundFile sonidoEspecial;

int activosAnterior = 0; 

void setup() {
  size(400, 200);
  
  // Imprime la lista de puertos para asegurarte de seleccionar el correcto
  printArray(Serial.list()); 
  
  // Se selecciona el primer puerto de la lista. Asegúrate que Arduino esté ahí.
  String portName = Serial.list()[0]; 
  miPuerto = new Serial(this, portName, 9600);
  miPuerto.clear(); // Limpia cualquier residuo inicial del puerto
  miPuerto.bufferUntil('\n'); 

  // Cargar sonido ambiente en Loop
  sonidoAmbiente = new SoundFile(this, "ambiente.mp3");
  sonidoAmbiente.loop();
  sonidoAmbiente.amp(1.0); 

  // Cargar arrays de sonidos (Índices del 0 al 4)
  for (int i = 0; i <= 4; i++) {
    sonidosActivar[i] = new SoundFile(this, "activar" + i + ".mp3");
    sonidosDesactivar[i] = new SoundFile(this, "desactivar" + i + ".mp3");
  }
  
  sonidoEspecial = new SoundFile(this, "especial.mp3");
}

void draw() {
  background(40);
  fill(255);
  textSize(18);
  text("Sensores Activos Anteriormente: " + activosAnterior, 30, 80);
}

// Este evento se dispara automáticamente al recibir un '\n'
void serialEvent(Serial miPuerto) {
  String datoString = miPuerto.readStringUntil('\n');
  
  if (datoString != null) {
    datoString = trim(datoString); 
    
    // Validar que la cadena no esté vacía antes de convertir
    if (datoString.length() > 0) {
      try {
        int activosActual = int(datoString);
        
        // Solo actuar si hay un cambio real de estado
        if (activosActual != activosAnterior) {
          gestionarSonidos(activosActual, activosAnterior);
          activosAnterior = activosActual; 
        }
      } catch (Exception e) {
        println("Error parseando dato: " + datoString);
      }
    }
  }
}

void gestionarSonidos(int actual, int anterior) {
  // 1. CONTROL DEL VOLUMEN AMBIENTE
  if (actual > 0) {
    sonidoAmbiente.amp(0.25); 
  } else {
    sonidoAmbiente.amp(1.0);  
  }

  // 2. LÓGICA DE DISPARO (ONE-SHOT)
  
  // CASO A: Todos los sensores activados
  if (actual == 4) {
    sonidoEspecial.play();
    return; 
  }
  
  // CASO B: Aumentaron los sensores activos
  if (actual > anterior) {
    if (actual >= 0 && actual <= 4) {
      sonidosActivar[actual].play();
    }
  } 
  
  // CASO C: Disminuyeron los sensores activos
  else if (actual < anterior) {
    if (actual >= 0 && actual <= 4) {
      sonidosDesactivar[actual].play();
    }
  }
}
