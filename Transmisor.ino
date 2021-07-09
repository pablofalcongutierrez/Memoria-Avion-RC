/***TRANSMISOR***/
#include <RF24.h>
#include <SPI.h>

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
}


//Declaramos incognitas para que el pulsador funcione como interruptor
float giro = 0.60;

//Para saber si esta activada la funcion de "vuelo suave"
uint8_t pinPulsador = 2;

//Esta incógnita nos va a decir si los alerones giran el 100% o el 60%
//AHORA PERTENECE A EL ARRAY joystick[4]
//uint8_t inicio = 0; --> joysticks[3]=0;
uint8_t estadoInicial = 1;
uint8_t estadoFinal = 1;

//Valores de los joysticks
uint32_t valorJoystickDX; //  Joystick derecho, eje x
uint32_t valorJoystickDY; //  Joystick derecho, eje y
uint32_t valorJoystickIX; //  Joystick izquierdo, eje x

//Pines analógico joysticks
uint8_t pinJoystickDX = 0; // Joystick derecho, eje X
uint8_t pinJoystickDY = 1; // Joystick derecho, eje y
uint8_t pinJoystickIX = 2; // Joystick izquierdo, eje X
//uint8_t pinJoystickIY = 3; // Joystick izquierdo, eje Y

int joysticks[4];
//Hasta que este el motor le daremos este valor al joystick
//joysticks[3] = 0;

/*Esta funcion tiene el fin de leer cada uno de los valores de los
joysticks y ademas saber cual es el estado del pulsador*/
void leerJoysticks(int joystick[]){
  //Lee el estado inicial del pulsador para ver si esta o no alimentado por la corriente
  estadoInicial = digitalRead(pinPulsador);
  
  if (estadoInicial && estadoFinal == 0){

    //Solo varia entre 1 y 0, nunca será negativo
    joysticks[3] = 1 - joysticks[3];
    delay(50);
  }

  estadoFinal = estadoInicial;
  
  valorJoystickDX = estabilizarValor(analogRead(pinJoystickDX));
  valorJoystickDY = estabilizarValor(analogRead(pinJoystickDY));
  valorJoystickIX = estabilizarValor(analogRead(pinJoystickIX));
  //valorJoystickIX = estabilizarValor(analogRead(pinJoystickIY));

  joysticks[0] = map(valorJoystickDX, 0, 1023, 0, 180);
  joysticks[1] = map(valorJoystickDY, 0, 1023, 0, 180);
  joysticks[2] = map(valorJoystickIX, 0, 1023, 0, 180);
  //Este pertenecera al motor
  //joysticks[3] = map(valorJoystickIY, 0, 1023, 0, ????);
}

//Creamos el objeto del tipo RF24
RF24 radio(8, 10);  //Pines CE y CSN

const byte identificacion[] = "000001";

void setup(){
  Serial.begin(9600);

  //Inicializamos el objeto radio
  radio.begin();

  //Configuramos los pines del led y del pulsador
  pinMode(pinPulsador, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //Utilizamos esta funcion para que el objeto radio pueda empezar enviar informacion
  radio.openWritingPipe(identificacion);

  /*Lo dejamos como minimo, si queremos mas rango usariamos RF24_PA_MAX
  aunque harian falta dos condensadores...*/
  radio.setPALevel(RF24_PA_MIN);

  //Para que solo actue como transmisor
  radio.stopListening();

  //Esta variable tiene que estar en el setup, no puede ir fuera de la funcion
  joysticks[3] = 0;
}

void loop(){
  
  leerJoysticks(joysticks);
  
  /*Se envia el puntero, es decir, la ubicacion de donde se guarda
  la variable texto y despues se envia el tamaño de la variable*/
  radio.write(&joysticks, sizeof(joysticks));
  delay(500);

  Serial.println("joysticks[0]\t|\tjoysticks[1]\t|\tjoysticks[2]\t|\tjoysticks[3]");
  
  Serial.print("\t");
  Serial.print(joysticks[0]);
  Serial.print("\t|\t\t");
  Serial.print(joysticks[1]);
  Serial.print("\t|\t\t");
  Serial.print(joysticks[2]);
  Serial.print("\t|\t\t");
  Serial.print(joysticks[3]);
  
  Serial.println();

}
