#define flowSensorPin 3 // Cambiar al pin conectado al sensor de flujo
#define relayPin 4 // Cambiar al pin conectado al relevador

volatile long NumPulsos; //variable para la cantidad de pulsos recibidos

void ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

void setup() {
  Serial.begin(9600);
  pinMode(flowSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Desactivar relevador (LOW LEVEL TRIGGER)
  
  attachInterrupt(0,ContarPulsos,RISING);//(Interrupción 0(Pin2),función,Flanco de subida)
  interrupts();    //Habilitamos las interrupciones
}

void loop() {
  // Para este ejemplo, asumiré que quieres llamar a dispenseLiquid() directamente en el código.
  // Podrías reemplazar esto con cualquier lógica o condición bajo la cual quieras dispensar líquido.

  if(Serial.available())
  {
    char opc = Serial.read();
    dispenseLiquid(opc);
  }
  
  // Ejemplo de uso:
  // dispenseLiquid(1); // Para dispensar 60ml
  // delay(10000); // Espera 10 segundos antes de la próxima operación, ajusta según necesites

  // Este es solo un ejemplo, deberías implementar tu propia lógica para decidir cuándo y cómo llamar a dispenseLiquid().
}

void dispenseLiquid(char liquid) {
  int targetPulses = 0;
  switch (liquid) {
    case '1': targetPulses = calculatePulses(60);  break;
    case '2': targetPulses = calculatePulses(120); break;
    case '3': targetPulses = calculatePulses(250); break;
    case '4': targetPulses = calculatePulses(500); break;
    case '5': targetPulses = calculatePulses(1000); break;
    case '6': digitalWrite(relayPin, HIGH); return; // Paro de emergencia
  }

  targetPulses-=6;
  digitalWrite(relayPin, LOW); // Activar relevador
  NumPulsos = 0;
  while(NumPulsos < targetPulses) {
    // Esperar a que se alcance el número objetivo de pulsos
    Serial.print("ML Surtidos: ");
    Serial.println(NumPulsos/0.368);
    delay(100);
  }
  Serial.print("ML Surtidos: ");
  Serial.println(targetPulses/0.368);
  digitalWrite(relayPin, HIGH); // Desactivar relevador
}


int calculatePulses(int ml) {
  // Aquí deberás insertar la fórmula basada en tu calibración específica entre ml y pulsos.
  // Esta es una fórmula de ejemplo y deberás ajustarla según tus mediciones.
  // Por ejemplo, si tu sensor emite 450 pulsos por litro, entonces por cada ml serían 0.45 pulsos.
  // Asegúrate de calibrar tu sensor para obtener esta relación.
  return ml * 0.368;
}