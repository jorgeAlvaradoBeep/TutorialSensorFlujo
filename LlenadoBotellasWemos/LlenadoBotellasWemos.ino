#define flowSensorPin D2 // Cambiar al pin conectado al sensor de flujo
#define relayPin D1 // Cambiar al pin conectado al relevador
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int ledPin = LED_BUILTIN; // Usar el LED integrado

const char* ssid = "Tostatronic"; // Reemplaza con el nombre de tu red WiFi
const char* password = "Jorge19951$"; // Reemplaza con la contraseña de tu red WiFi

ESP8266WebServer server(80); // El servidor web escuchará en el puerto 80

volatile long NumPulsos; //variable para la cantidad de pulsos recibidos

void ICACHE_RAM_ATTR ContarPulsos ()
{ 
  NumPulsos++;  //incrementamos la variable de pulsos
} 

void setup() {
  Serial.begin(9600);
  pinMode(flowSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Desactivar relevador (LOW LEVEL TRIGGER)
  pinMode(ledPin, OUTPUT); // Inicializa el pin del LED como salida
  digitalWrite(ledPin, HIGH); // Asegúrate de que el LED está apagado al inicio
  
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), ContarPulsos, RISING);
  interrupts();    //Habilitamos las interrupciones
  NumPulsos=0;

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 10000) { // Si pasan más de 10 segundos
      blinkLed(); // Comenzar a parpadear el LED como señal de error
      return; // Sale de setup para no continuar intentando conectar o iniciar el servidor
    }
  }

  // Si la conexión fue exitosa, enciende el LED integrado
  Serial.println("");
  Serial.print("Conectado a ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(ledPin, LOW); // Enciende el LED integrado

  // Configura el servidor web
  setupServer();
}

void loop() {
  server.handleClient(); // Maneja las peticiones al servidor web
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
  Serial.print("Terminado");
  digitalWrite(relayPin, HIGH); // Desactivar relevador
}


int calculatePulses(int ml) {
  // Aquí deberás insertar la fórmula basada en tu calibración específica entre ml y pulsos.
  // Esta es una fórmula de ejemplo y deberás ajustarla según tus mediciones.
  // Por ejemplo, si tu sensor emite 450 pulsos por litro, entonces por cada ml serían 0.45 pulsos.
  // Asegúrate de calibrar tu sensor para obtener esta relación.
  return ml * 0.368;
}

void setupServer() {
  // Configura las rutas del servidor web
  server.on("/control", HTTP_GET, []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      if (state == "1") {
        dispenseLiquid('1');
        server.send(200, "text/plain", "Botella de 60ml Dispensada");
      } else if (state == "2") {
        dispenseLiquid('2');
        server.send(200, "text/plain", "Botella de 120ml Dispensada");
      }else if (state == "3") {
        dispenseLiquid('3');
        server.send(200, "text/plain", "Botella de 250ml Dispensada");
      }else if (state == "4") {
        dispenseLiquid('4');
        server.send(200, "text/plain", "Botella de 500ml Dispensada");
      }else if (state == "5") {
        dispenseLiquid('5');
        server.send(200, "text/plain", "Botella de 1L Dispensada");
      }
      else
        server.send(400, "text/plain", "Argumento Erroneo");
    } else {
      server.send(400, "text/plain", "Argumento faltante: state");
    }
  });

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void blinkLed() {
  // Función para parpadear el LED cada segundo
  while(true) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
  }
}