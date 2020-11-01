#include <ESP8266WiFi.h>

const char *ssid = "YOUR SSID";  
const char *pass = "YOUR INTERNET PASSWORD"  

#define LED D0 

;WiFiServer server(80); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // put your main code here, to run repeatedly:

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED, HIGH);
    value = HIGH;
  }

  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED, LOW);
    value = LOW;
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("LED pin is now: ");

  if(value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin D0 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin D0 OFF<br>");

  delay(1);
  Serial.println("Client Disconnected");
  Serial.println("");

}
