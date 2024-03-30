#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Tostatronic"; // Reemplaza con el nombre de tu red WiFi
const char* password = "Jorge19951$"; // Reemplaza con la contraseña de tu red WiFi

ESP8266WebServer server(80); // El servidor web escuchará en el puerto 80

const int ledPin = LED_BUILTIN; // Usar el LED integrado
const int relay = D1;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Inicializa el pin del LED como salida
  digitalWrite(ledPin, HIGH); // Asegúrate de que el LED está apagado al inicio

  pinMode(relay, OUTPUT); // Inicializa el pin del LED como salida
  digitalWrite(relay, HIGH); // Asegúrate de que el relevador permanezca apagado.

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

void setupServer() {
  // Configura las rutas del servidor web
  server.on("/control", HTTP_GET, []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      if (state == "1") {
        digitalWrite(relay, LOW); // Enciende el LED
        server.send(200, "text/plain", "LED Encendido");
      } else if (state == "0") {
        digitalWrite(relay, HIGH); // Apaga el LED
        server.send(200, "text/plain", "LED Apagado");
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

