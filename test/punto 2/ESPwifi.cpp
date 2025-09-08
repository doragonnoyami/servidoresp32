#include <WiFi.h>

// --- Configuración de Red ---
const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// --- Configuración del Servidor y Hardware ---
WiFiServer server(80); // Puerto 80 es el estándar para HTTP
int seg7 [7] = {0,1,2,3,4,5,6};  // Pin del LED integrado en la mayoría de placas ESP32
int seg72 [7] = {7,8,9,10,11,12,13};  // Pin del LED integrado en la mayoría de placas ESP32
int numeros [10][7] = {
    {1,1,1,1,1,1,0}, //0
    {1,1,0,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,0,1}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,0,0,1,1} //9
};
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
    html { font-family: sans-serif; text-align: center; }
    .btn { display: flex; background-color:rgb(255, 230, 0); border: none; color: white;
           padding: 16px 30px; text-decoration: none; font-size: 24px; margin: 2px; cursor: pointer; border-radius: 8px; }
    .btn-a { display: flex; background-color:rgb(25, 0, 255); }
    .btn-b { background-color: #f44336; }
    .btn-c { background-color: #f44336; }
    .btn-d { background-color: #f44336; }
    .btn-e { background-color: #f44336; }
    .btn-f { background-color: #f44336; }
    .btn-g { background-color: #f44336; }
    .btn-h { background-color: #f44336; }
    .btn-i { background-color: #f44336; }
    .btn-j { background-color: #f44336; }
    .btn-k { background-color: #f44336; }
  </style>
</head>
<body>
  <h1>Servidor Web</h1>
  <p>Control del LED interno</p>
  <p>
    <a href="/one"><button class="btn">uno</button></a>
  </p>
  <p>
    <a href="/two"><button class="btn btn-a">dos</button></a>
  </p>
  <p>
    <a href="/cero"><button class="btn btn-b">apagar</button></a>
  </p>
  <p>
    <a href="/uno"><button class="btn btn-c">apagar</button></a>
  </p>
  <p>
    <a href="/dos"><button class="btn btn-d">apagar</button></a>
  </p>
  <p>
    <a href="/tres"><button class="btn btn-e">apagar</button></a>
  </p>
  <p>
    <a href="/cuatro"><button class="btn btn-f">apagar</button></a>
  </p>
  <p>
    <a href="/cinco"><button class="btn btn-g">apagar</button></a>
  </p>
  <p>
    <a href="/seis"><button class="btn btn-h">apagar</button></a>
  </p>
  <p>
    <a href="/siete"><button class="btn btn-i">apagar</button></a>
  </p>
  <p>
    <a href="/ocho"><button class="btn btn-j">apagar</button></a>
  </p>
  <p>
    <a href="/nueve"><button class="btn btn-k">apagar</button></a>
  </p>
</body>
</html>
)rawliteral";
int i=0;
int ta=0, td=1000, tp=0;
int j=0;
void setup() {

  Serial.begin(115200);
  for(i=0;i<7;i++){
    pinMode (seg7[i],OUTPUT);
    digitalWrite(seg7[i],LOW);
    pinMode (seg72[i],OUTPUT);
    digitalWrite(seg72[i],OUTPUT);
  }
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
          if(header.indexOf("GET/one")>=0){
            if (header.indexOf("GET /cero") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[0],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /uno") >= 0) {
                Serial.println("Request para secuencia 1");
                  for(i=0;i<7;i++){
                    digitalWrite(seg7[1],numeros[0][i]);
                  }
              }
            else if (header.indexOf("GET /dos") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[2],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /tres") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[3],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /cuatro") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[4],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /cinco") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[5],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /seis") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[6],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /siete") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[7],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /ocho") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[8],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /nueve") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[9],numeros[0][i]);
                }
            }
          }
          else if(header.indexOf("GET/two")>=0){
            if (header.indexOf("GET /cero") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[0],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /uno") >= 0) {
                Serial.println("Request para secuencia 1");
                  for(i=0;i<7;i++){
                    digitalWrite(seg7[1],numeros[0][i]);
                  }
              }
            else if (header.indexOf("GET /dos") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[2],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /tres") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[3],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /cuatro") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[4],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /cinco") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[5],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /seis") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[6],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /siete") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[7],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /ocho") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[8],numeros[0][i]);
                }
            }
            else if (header.indexOf("GET /nueve") >= 0) {
              Serial.println("Request para secuencia 1");
                for(i=0;i<7;i++){
                  digitalWrite(seg7[9],numeros[0][i]);
                }
            }
          }
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