import processing.serial.*;
import 

Serial myPort;
String mensaje; //datos del tipo sting recibidos por el Puerto serial

void preload(){
};

void setup(){
//En Windows el primer Puerto serial es COM1 => Serial.list()[0]
String portName = Serial.list()[0];
myPort = new Serial (this, portName, 9600);

//sonidos
String [] efectosApertura = new String[5];
String [] efectosCierre = new String[5];
String ambiente;
};

void draw(){
if (myPort.available() > 0){
  mensaje = myPort.readStringUntil('\n');
};
println(mensaje);


};
