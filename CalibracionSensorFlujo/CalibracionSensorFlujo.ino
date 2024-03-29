volatile long NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 3;    //Sensor conectado en el pin 2


//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

//---Función para obtener frecuencia de los pulsos--------

void setup()
{ 
  Serial.begin(9600); 
  pinMode(PinSensor, INPUT); 
  attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  interrupts();    //Habilitamos las interrupciones
} 

void loop ()    
{
  //-----Enviamos por el puerto serie---------------
  Serial.print ("Numero de Pulsos = "); 
  Serial.println (NumPulsos); 
  delay(100);
  if(Serial.available())
  {
    char c = Serial.read();
    if(c=='1')
      NumPulsos=0;
  }
}