/* Juego de la Serpiente
*Material
*Arduino mega
*Matriz 16x16
*Joysticks
*/

//Varible que recoge la posición del alimento
byte alimento[1][2];
//Variable que recoge la posición de la serpuente
byte posicion[100][2];
//Variable que recoge la longitud de la serpiente
byte longitud;
//Varables de lectura de los ejer del Joysticks
int x=512;
int y=512;
//Variable que recoge la dirección del desplazamiento
byte direccion;
//Variable auxiliar para el control del tiempo
long time;
//Función para inicialidar el valor inicial de las variables
void setup(){  
  for(byte i=0; i<=15; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);  //Al ser de anodo común ponemos los cátodos en 1 para que no esten encendidos.
  }

  for(byte i=22; i<=37; i++){
    pinMode(i, OUTPUT);    
  }
  direccion=0;
  posicion[0][0]=8;
  posicion[0][1]=7;
  longitud=1;
  time=0;
  randomSeed(analogRead(A2));
  alimento[0][0]= random(0, 16);
  alimento[0][1]= random(0, 16);
}

//Función principal del programa
void loop(){
  //Dibuja en la matriz el cuerpo y alimento
  Escribir_matriz();
  //ejecuta el siguiente bloque de funciones cada 150 milisegundos
  if(millis() >= time+150){  
    time=millis();
    //Lee lo valores del joysticks
    Leer_joysticks();
    //Cambia la dirección del desplazamiento
    Cambio_direccion();
    //Cambia la posición de la serpiente
    Cambio_posicion();
    //Comprueba que la cabeza no entre en contacto con el cuerpo 
    Control_muerte();
    //Comprueba que la cabeza este enla posición del alimento
    Comprobar_alimento();
  }
}

void Leer_joysticks(){

  x=analogRead(A1);
  y=analogRead(A0);
}

void Cambio_direccion(){
  if( direccion==0 || direccion==1 ){
    if(y>900){
      direccion=2;
      return ;
    }
    if(y<100){
      direccion=3;
      return ;
    }
  }

  if( direccion==3 || direccion==2 ){
    if(x>900){
      direccion=0;
      return ;
    }
    if(x<100){
      direccion=1;
      return ;
    }
  }

}
void Cambio_posicion(){
  for(byte i=longitud; i>1; i--){
    posicion[i-1][0]=posicion[i-2][0];
    posicion[i-1][1]=posicion[i-2][1];  
  }

  if(direccion==2){
    if(posicion[0][0]==15){
      posicion[0][0]=0;
      return ;
    }
    posicion[0][0]++;
    return ;
  }

  if(direccion==3){
    if(posicion[0][0]==0){
      posicion[0][0]=15;
      return ;
    }
    posicion[0][0]--;
    return ;
  }
  if(direccion==0){
    if(posicion[0][1]==15){
      posicion[0][1]=0;
      return ;
    }
    posicion[0][1]++;
    return ;
  }

  if(direccion==1){
    if(posicion[0][1]==0){
      posicion[0][1]=15;
      return ;
    }
    posicion[0][1]--;
    return ;
  }

}

void Escribir_matriz(){
  for(int i=longitud; i>=0; i--){
    digitalWrite( posicion[i-1][0]+22, HIGH);
    digitalWrite( posicion[i-1][1], LOW);
    digitalWrite( posicion[i-1][1], HIGH);
    digitalWrite( posicion[i-1][0]+22, LOW);

  }
  digitalWrite( alimento[0][0]+22, HIGH);
  digitalWrite( alimento[0][1], LOW);
  digitalWrite( alimento[0][0]+22, LOW);
  digitalWrite( alimento[0][1], HIGH);
}

void Comprobar_alimento(){
  if(posicion[0][0]==alimento[0][0]){
    if(posicion[0][1]==alimento[0][1]){
      Cambio_posicion();
      //Función encargada de aumentar el tamaño de la serpiente
      crecer();
      alimento[0][0]= random(0, 16);
      alimento[0][1]= random(0, 16);            
    }
  }
}

void crecer(){
  longitud++;
  posicion[longitud-1][0]=alimento[0][0];
  posicion[longitud-1][1]=alimento[0][1];
}

void Control_muerte(){

  for( byte i=longitud; i>1; i--){
    if(posicion[i-1][0]==posicion[0][0]){
      if(posicion[i-1][1]==posicion[0][1]){
        Escribir_matriz();
        muerte();
      }
    }
    if(longitud==1){
      break;
    } 
  }
}
//Función encargada de reestablecer lo valores inciales tras la muerte
void muerte(){
  longitud=1;
  posicion[0][0]=8;
  posicion[0][0]=7;
  alimento[0][0]= random(0, 16);
  alimento[0][1]= random(0, 16);
}


