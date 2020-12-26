  byte latchPin = 5;  //al pin 12 del shift register 74hc595
  byte clockPin = 4;  //al pin 11
  byte dataPin  = 2;  //al pin 14
  byte posicion [64][2]; // poscion de cada elemento de la serpiente
  byte alimento [2]; //ubicacion del alimento
  byte direccion; // direccion en la que se mueve la serpiente
  byte longitud; // cantidad de elementos del cuerpo de la serpiente
  byte velocidad; // velocidad de movimiento de la serpiente
  
  int x; // valor de entrada joystick eje x
  int y; // valor joystick eje y

  long time; // tiempo desde que inicio el juego
  
void setup()
{
  // definir las salidas de informacion para llenar la matriz led con shift register 74hc595
  pinMode(latchPin, OUTPUT); // gatillo
  pinMode(clockPin, OUTPUT); // reloj, el mismo para todos los shift registers
  pinMode(dataPin, OUTPUT);  // datos, se arrojan en cascada

  randomSeed( analogRead(A2)); //se elije una semilla leyendo una entrada al aire, los valores leidos son aleatorios
  alimento[0] = random(0,8);
  alimento[1] = random(0,8);
  
  direccion = 0; // movimiento inicial a la derecha(?)
  posicion[0][0] = 0;
  posicion[0][1] = 0;
  time = 0;
  longitud = 1;
  velocidad = 200;
}

void loop()
{
  imprimirSerpiente();
  imprimirAlimento();
  if(millis() >= time+velocidad)
  {
    time=millis();
    leerJoystick();
    cambioDireccion();
    cambioPosicion();
    meMordi();
    comer();
  }

}
void imprimirSerpiente() // metodo implementado para usar 2 shift register 74hc595 (matriz led 8x8)
{
  for(int i = longitud-1; i>=0 ; i--) // imprime cada elemento del cuerpo de la serpiente un punto a la vez
  {
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ~(byte)(pow(2,posicion[i][1])+0.01));
  shiftOut(dataPin, clockPin, MSBFIRST, (byte)(pow(2,posicion[i][0])+0.01));
  digitalWrite(latchPin, HIGH);
  }
}

void imprimirAlimento()
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ~(byte)(pow(2,alimento[1])+0.01));
  shiftOut(dataPin, clockPin, MSBFIRST, (byte)(pow(2,alimento[0])+0.01));
  digitalWrite(latchPin, HIGH);
}

void leerJoystick() // captura los valores (entre 0 y 1024) de voltajes en cada entrada analoga
{
  x=analogRead(A1);
  y=analogRead(A0);
}

void cambioDireccion()
{
  if(direccion==0 || direccion==1)
  {
    if(y>800)
    {
      direccion=2;
      return;
    }
    if(y<200)
    {
      direccion=3;
      return;
    }
  }
  else if(direccion==2 || direccion==3)
  {
      if(x>800)
    {
      direccion=0;
      return;
    }
    if(x<200)
    {
      direccion=1;
      return;
    }
  }
  
}

void cambioPosicion() //Mueve la cabeza de la serpiente a una nueva posicion de acuerdo a las lecturas del joystick y mueve cada parte del cuerpo siguiendo la cabeza.
{
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 255); // Apaga toda la matriz para desdibujar lo que existia
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);

  for(int i = longitud; i>1 ; i--)
  {
    posicion[i-1][0]=posicion[i-2][0];
    posicion[i-1][1]=posicion[i-2][1];
  }

  if(direccion==2)
  {
    if(posicion[0][1]==7)
    {
      posicion[0][1]=0;
      return;
    }
    posicion[0][1]++;
    return;
  }
  if(direccion==3)
  {
    if(posicion[0][1]==0)
    {
      posicion[0][1]=7;
      return;
    }
    posicion[0][1]--;
    return;
  }
    if(direccion==0)
  {
    if(posicion[0][0]==7)
    {
      posicion[0][0]=0;
      return;
    }
    posicion[0][0]++;
    return;
  }
    if(direccion==1)
  {
    if(posicion[0][0]==0)
    {
      posicion[0][0]=7;
      return;
    }
    posicion[0][0]--;
    return;
  }
}

void comer() // Si la cabeza esta en la posicion del alimento entonces lo consume y genera nuevo alimento en una posicion aleatoria.
{
  if(posicion[0][0]==alimento[0])
  {
    if(posicion[0][1]==alimento[1])
    {
      cambioPosicion();
      crecer();
      alimento[0]=random(0,8);
      alimento[1]=random(0,8);
    }
  }
}

void crecer() // Aumenta la longitud de la serpiente y coloca el nuevo elemento del cuerpo en la posicion en que estaba el alimento consumido.
{
  longitud++;
  posicion[longitud-1][0] = alimento[0];
  posicion[longitud-1][1] = alimento[1];
}

void meMordi() // Revisa cuando la serpiente se muerde a si misma, si es el caso termina el juego.
{
  for(byte i=longitud; i>1;i--)
    if(posicion[i-1][0]==posicion[0][0])
      if(posicion[i-1][1]==posicion[0][1])
      {
        imprimirSerpiente();
        muerte();
      }
}

void muerte() // reinicia el juego
{
  longitud=1;
  posicion[0][0]=0;
  posicion[0][1]=0;
  alimento[0]=random(0,8);
  alimento[1]=random(0,8);
}
