#include <Arduino.h>
// Local libraries
#include "http.h"
#include "get_vals.h"
#include "start.h"
// External libraries
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

  
// Definitions
#define LED 2
String ssid = "123";
String password = "12345678";

// Variables
unsigned long lastTime = 0;
unsigned long timerDelay = 60000; // delay is 16 hours
JsonDocument doc;

String serverName = "https://www.hvakosterstrommen.no/api/v1/prices/2024/02-02_NO3.json"; // API endpoint

// DASHBOARD
AsyncWebServer server(80);
ESPDash dashboard(&server);
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");


void setup() {
	Serial.begin(115200);
	startWifi(ssid, password);
	server.begin();
	pinMode(LED,OUTPUT);
	digitalWrite(LED,HIGH);
	}

void loop() {
	// HTTP Request
	HTTPClient http;
	String serverPath = serverName;
	http.begin(serverPath.c_str());
	int httpResponseCode = http.GET();

	// HTTP Response and parsing
	if (httpResponseCode>0) {
		while ( (WiFi.status() == WL_CONNECTED) && ((millis() - lastTime) < timerDelay) ) {
			digitalWrite(LED,HIGH); // WIFI indicator
			respCode(httpResponseCode);

			String payload = http.getString();

			deserializeJson(doc,payload);

			printVals(doc,"NOK_per_kWh");
			printVals(doc,"EUR_per_kWh");
			temperature.update((int)random(0, 50));
			humidity.update((int)random(0, 100));
			/* Send Updates to our Dashboard (realtime) */
			dashboard.sendUpdates();

			delay(10000);
			}
		}
		else {
			respCode(httpResponseCode);
			delay(60000); // wait 1 minute
		}
		if (WiFi.status() != WL_CONNECTED) {
			digitalWrite(LED,LOW); // WIFI indicator
			Serial.println("WiFi Disconnected");

		}
		lastTime = millis();
		http.end(); // Free resources
	}