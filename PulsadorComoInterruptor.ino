//pines del led y pulsador
int pinLed = 13;
int pinPulsador = 2;

int inicio = 0;
int estadoInicial = 1;
int estadoFinal = 1;

void setup(){
  Serial.begin(9600);
  pinMode(pinPulsador, INPUT);
  pinMode(pinLed, OUTPUT);
}

void loop(){

  estadoInicial = digitalRead(pinPulsador);

  //Los simbolos && se comportan como un por
  if(estadoInicial && estadoFinal == 0){
    inicio = 1 - inicio;
    delay(50);
  }
  Serial.println(inicio);
  estadoFinal = estadoInicial;

  digitalWrite(pinLed, inicio);
}
