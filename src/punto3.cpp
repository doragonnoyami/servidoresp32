#include <WiFi.h>
#include <WebServer.h>
// --- Configuración de Red ---
const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// --- Configuración del Servidor y Hardware ---
WiFiServer server(80); // Puerto 80 es el estándar para HTTP
const int ledPin = 2;  // Pin del LED integrado en la mayoría de placas ESP32
int leds[3] = {0,1,2}; //v a r
String modo;
/*
◦ Título de primer nivel con leyenda “Semáforo”, todo con mayúsculas, con fuente ‘Impact’, negrita y subrayada de 45px. Utilizar la clase “titulo”.
◦ Color del fondo del cuerpo blanco.
◦ Todo centrado.
◦ 4 botones, uno debajo del otro, de ancho 200px y alto 200px, de color de fondo rojo (lento), amarillo (normal), verde (rápido) y naranja (emergencia); color de texto negro con 
tamaño de fuente de 23px, borde negro de 3px sólido de radio 100px. Utilizar clase genérica “boton” y clases individuales “lento”, “normal”, “rápido” y “emergencia”.
◦ Abajo de todo agregar una sección (span) con la leyenda “Modo: XXX” y reemplazar el XXX por el modo seleccionado en cuestión, el modo solo en negrita.

*/
// --- Página Web ---
// Se guarda en memoria de programa (PROGMEM) para ahorrar RAM. Es más eficiente que usar un String.
const char pagina_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Control LED ESP32</title>
  <style>
    body {
    justify-content: center;
    background-color: white;
    }
    html { font-family: sans-serif; text-align: center; }
    .rapido{ display: inline-block; background-color: #4CAF50; border: 3px solid black; color: white; widht: 200px; height: 200px
           padding: 16px 30px; text-decoration: none; font-size: 23px; margin: 2px; cursor: pointer; border-radius: 100px; }
    .medio { background-color:rgb(251, 255, 0); border 3px solid black: none; color: white;widht: 200px; height: 200px
           padding: 16px 30px; text-decoration: none; font-size: 23px; margin: 2px; cursor: pointer; border-radius: 100px; }
    .lento{ display: inline-block; background-color:rgb(255, 0, 0); border 3px solid black: none; color: white;widht: 200px; height: 200px
           padding: 16px 30px; text-decoration: none; font-size: 23px; margin: 2px; cursor: pointer; border-radius: 100px; }
    .emergencia { background-color:rgb(255, 123, 0); border 3px solid black: none; color: white;widht: 200px; height: 200px
           padding: 16px 30px; text-decoration: none; font-size: 23px; margin: 2px; cursor: pointer; border-radius: 100px; }
    .titulo {
        font-family: Impact;
        font-size: 45px;
        text-weight: bold;
        text-decoration: underline;
    }
  </style>
</head>
<body>
    <div class="titulo">
      <h1>SEMAFORO</h1>
        <p> SELECCIONE EL MODO DE SU SEMAFORO.</p>
    </div>
    <div class="boton">
        <p>
    <a href="/lento"><button class="btn lento">lento</button></a>
  </p>
  <p>
    <a href="/medio"><button class="btn medio">medio</button></a>
  </p>
  <p>
    <a href="/lapido"><button class="btn rapido">lapido</button></a>
  </p>
  <p>
    <a href="/emelgentia"><button class="btn emergencia">emelgentia</button></a>
  </p>
    </div>
     <p>XXX</a></p>
</body>
</html>
)rawliteral";


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // --- Conexión a la red Wi-Fi ---
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20; // 20 intentos de 500ms = 10 segundos
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }

  // --- Iniciar Servidor ---
  Serial.println("\nConexion Wi-Fi exitosa!");
  Serial.print("IP del servidor: http://");
  Serial.println(WiFi.localIP());
  server.begin();
  
}


void loop() {
  // 1. Esperar a que un cliente se conecte
  WiFiClient client = server.available();
  if (!client) {
    return; // Si no hay cliente, no hacer nada y salir del loop
  }

  Serial.println("\n[Nuevo Cliente Conectado]");

  // 2. Leer la petición del cliente línea por línea
  String header = "";           // Para guardar la petición HTTP completa
  String currentLine = "";      // Para guardar la línea actual que estamos leyendo

  // El bucle se ejecuta mientras el cliente esté conectado
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();   // Leer un caracter a la vez
      header += c;              // Añadirlo a la petición completa

      // Si el caracter es un salto de línea, procesamos la línea
      if (c == '\n') {
        // Una línea en blanco (dos saltos de línea seguidos) indica el fin de la petición HTTP
        if (currentLine.length() == 0) {

          // 3. Interpretar la petición y actuar
          if (header.indexOf("GET /lapido") >= 0) {
            modo= "Rapido";
            Serial.println("semaforo modo lapido");
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[0],HIGH);
            delay(40000);
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[1],HIGH);
            delay(1000);
            digitalWrite(leds[1],LOW);
            digitalWrite(leds[2],HIGH);
            delay(20000);
          } else if (header.indexOf("GET /medio") >= 0) {
            modo= "Medio";
            Serial.println("semaforo modo medio");
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[0],HIGH);
            delay(65000);
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[1],HIGH);
            delay(2000);
            digitalWrite(leds[1],LOW);
            digitalWrite(leds[2],HIGH);
            delay(45000);
          }  else if (header.indexOf("GET /lento") >= 0) {
            modo= "Lento";
            Serial.println("semaforo modo lento");
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[0],HIGH);
            delay(50000);
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[1],HIGH);
            delay(3000);
            digitalWrite(leds[1],LOW);
            digitalWrite(leds[2],HIGH);
            delay(70000);
          } else if (header.indexOf("GET /emelgentia") >= 0) {
            modo= "Emergencia";
            Serial.println("semaforo modo emelgentia");
            digitalWrite(leds[2],LOW);
            digitalWrite(leds[1],HIGH);
            delay(1500);
            digitalWrite(leds[1],LOW);
            digitalWrite(leds[2],HIGH);
            delay(1500);
          } 
          String pagina;
          pagina.replace("XXX", modo);

          // 4. Enviar la respuesta HTTP (construida manualmente)
          // Encabezado de la respuesta
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println(); // Esta línea en blanco es OBLIGATORIA para separar encabezado y cuerpo

          // Cuerpo de la respuesta (la página web)
          client.print(pagina_html);

          // Salimos del bucle 'while' ya que hemos respondido
          break;
        } else {
          // Si no es una línea en blanco, la reseteamos para leer la siguiente
          currentLine = "";
        }
      } else if (c != '\r') {
        // Si no es un salto de línea ni un retorno de carro, lo añadimos a la línea actual
        currentLine += c;
      }
    }
  }

  // 5. Cerrar la conexión
  client.stop();
  Serial.println("[Cliente Desconectado]");
}