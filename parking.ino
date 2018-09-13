#include <SPI.h>
#include <Ethernet.h>
// Escriba una dirección MAC y la dirección IP para el controlador.
// La dirección IP será dependiente de la red local:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 200); // ip de la página a servir


// Inicializar la librería de servidor Ethernet
// Con la dirección IP y el puerto que desee utilizar
// (Puerto 80 es el valor predeterminado para HTTP):
EthernetServer server(80); void setup() {
// Abre el puerto serie de comunicacion y espera:
 Serial.begin(9600);
 while (!Serial) {
    ; // poner aquí un delay de espera para que se conecte el puerto. Solo necesario para
    // Arduino Leonardo
 }


 // inicia la conexión y el servidor:
 Ethernet.begin(mac, ip);
 server.begin();
 Serial.print("server is at ");
 Serial.println(Ethernet.localIP());
}


void loop() {
 // Detectar los clientes entrantes
 EthernetClient client = server.available();
 if (client) {
    Serial.println("Nuevo cliente");
    // Una petición http termina con una línea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
     if (client.available()) {
       char c = client.read();
       Serial.write(c);
       // Si se ha llegado al final de la línea recibirá una nueva línea
       // con un carácter en blanco, la petición http ha terminado,
       // Para que pueda enviar una respuesta
       if (c == '\n' && currentLineIsBlank) {
         // send a standard http response header
         client.println("HTTP/1.1 200 OK");
         client.println("Content-Type: text/html");
         client.println("Conexion Cerrada: cerrada");
         client.println("Refresh: 2");  // refresca cada 2 seg los valores
         client.println();
         client.println("<!DOCTYPE HTML>");
         client.println("<html>");
         client.print("PARQUEADERO ");
         client.print("UDLA");
         client.println("<br />");
// Añadir una etiqueta para conseguir que el navegador se actualice cada              //5 segundos: client.println("<meta http-equiv=\"refresh\" //content=\"5\">");
         // salida del valor de cada entrada analogica
         for (int analogChannel = 0; analogChannel < 4; analogChannel++) {
           int sensorReading = analogRead(analogChannel);
           client.print("Parqueadero numero: ");
           client.print(analogChannel); client.print(" se encuentra ");
           if(sensorReading>1000){
           client.print("ocupado ");}
           else{
           client.print("libre");}
           //client.print(sensorReading);
           client.println("<br />");
         }
         client.println("</html>");
         break;
       }
       if (c == '\n') {
         // se inicia una nueva línea
         currentLineIsBlank = true;
       }
       else if (c != '\r') {
         // ha llegado a un carácter en la línea actual
         currentLineIsBlank = false;
               }
         }
    }
    // facilita al navegador web un tiempo para recibir los datos
    delay(1);
    // cierra la conexión:
    client.stop();
    Serial.println("Cliente desconectado");
 }
}
