#include <Arduino.h>
// exsternal libraries
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <ArduinoJson.h>

// local libraries
#include "http.h"
#include "get_vals.h"
#include "start.h"


#define LED 2
int heat = 4; 
int CO = 34;

float COData; 

// sensor objects
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; 
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();


// Replace with your network credentials
const char* ssid = "Fairphone";
const char* password = "TEMPPASS";

// Set web server port number to 80
WiFiServer server(80);
String header;

String serverName = "https://www.hvakosterstrommen.no/api/v1/prices/2024/02-02_NO3.json"; // API endpoint
JsonDocument doc;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize sensor and LED
  pinMode(LED,OUTPUT);
  pinMode(heat, OUTPUT);
  bmp.begin();
  aht.begin();

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  digitalWrite(LED,HIGH);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients 
  HTTPClient http;
	String serverPath = serverName;
	http.begin(serverPath.c_str());
	int httpResponseCode = http.GET();
  
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      // GET NEW SENSOR VALUES
      sensors_event_t temp_event, pressure_event;
      bmp_temp->getEvent(&temp_event);
      bmp_pressure->getEvent(&pressure_event);
      sensors_event_t humidity, temp;
      //aht.getEvent(&humidity, &temp);  //problem here

      analogWrite(heat, 170); // set heating element i monoxide at 1.5v
      COData = analogRead(CO); 

      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            String payload = http.getString();

			      deserializeJson(doc,payload);
            String Pris = avgVals(doc,"NOK_per_kWh");

            // Display the HTML web page
            webPage(client,temp_event,pressure_event,COData,Pris);
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}