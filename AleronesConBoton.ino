#include<Servo.h>

//Corriente maxima con solo servos 0.07 mA


//Esta funcion tiene la mision de "estabilizar" los valores
//medidos en los potencimetros de los joysticks
int estabilizarValor(int X){
  if (X >= 480 && X <= 550){
    X = 512;
    return X;
  }

  if (X <= 13){
    X = 0;
    return X;
  }

  if (X >= 1014){
    X = 1023;
    return X;
  }

  return X;
}


//Declaramos incognitas para que el pulsador funcione como interruptor
float giro = 0.60;
int pinLed = 13;    //Para saber si esta activado
int pinPulsador = 2;
int inicio = 0;   //Esta incógnita nos va a decir si los alerones giran el 100% o el 70%
int estadoInicial = 1;
int estadoFinal = 1;

//Inicializamos los servos
Servo aleronDerecho;
Servo aleronIzquierdo;
Servo timon;
Servo elevador;

//Pines de los servos
int pinAleronDerecho = 12;
int pinAleronIzquierdo = 11;
int pinTimon = 10;
int pinElevador = 9;

//Establecemos los pulso a prueba-error
int PULSOMAX = 2450;
int PULSOMIN = 550;

//Pines de los dos joysticks
int pinJoystickDX = 0; // Joystick derecho, eje X
int pinJoystickDY = 1; // Joystick derecho, eje y
int pinJoystickIX = 2; // Joystick izquierdo, eje X

//Declaro las variables que vamos a utilizar para leer
//los valores de los joysticks
int valorJoystickDX;
int valorJoystickDY;
int valorJoystickIX;

//Declaro las variables que vamos a utilizar para angulos
int anguloJoystickDX;
int anguloJoystickDY;
int anguloJoystickIX;

void setup(){

  Serial.begin(9600);
  
  aleronDerecho.attach(pinAleronDerecho, PULSOMIN, PULSOMAX);
  aleronIzquierdo.attach(pinAleronIzquierdo, PULSOMIN, PULSOMAX);
  timon.attach(pinTimon, PULSOMIN, PULSOMAX);
  elevador.attach(pinElevador, PULSOMIN, PULSOMAX);

  pinMode(pinPulsador, INPUT);
  pinMode(pinLed, OUTPUT);
}

void loop(){

  //Lee el estado inicial del pulsador para ver si esta o no alimentado por la corriente
  estadoInicial = digitalRead(pinPulsador);
  
  if (estadoInicial && estadoFinal == 0){

    //Solo varia entre 1 y 0, nunca sera negativo
    inicio = 1 - inicio;
    delay(50);
  }

  estadoFinal = estadoInicial;
  
  valorJoystickDX = estabilizarValor(analogRead(pinJoystickDX));
  valorJoystickDY = estabilizarValor(analogRead(pinJoystickDY));
  valorJoystickIX = estabilizarValor(analogRead(pinJoystickIX));
  
  anguloJoystickDX = map(valorJoystickDX, 0, 1023, 0, 180);  //Angulo entre 0 y 180, leido en el JDX
  anguloJoystickDY = map(valorJoystickDY, 0, 1023, 0, 180);  //Angulo entre 0 y 180, leido en el JDY
  anguloJoystickIX = map(valorJoystickIX, 0, 1023, 0, 180);  //Angulo entre 0 y 180, leido en el JIX

  if(inicio == 1){
    //El servo se abrira un 50%
    anguloJoystickDX = (anguloJoystickDX - 90) * giro + 90;
    anguloJoystickDY = (anguloJoystickDY - 90) * giro + 90;
    //Es posible que no haga falta girar menos el timon
    //anguloJoystickIX = abs(anguloJoystickIX - 90) * giro + 90;

    //Para saber si esta activado el modo suave
    digitalWrite(pinLed, HIGH);
  }else{
    digitalWrite(pinLed, LOW);
  }

  Serial.println(inicio);
  
  aleronDerecho.write(anguloJoystickDX);
  aleronIzquierdo.write(180 - anguloJoystickDX);
  timon.write(anguloJoystickIX);
  elevador.write(anguloJoystickDY);
  
  delay(50);
}
