// include exsternal 
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// include local
#include "http.h"


// Replace with your network credentials
const char* ssid = "Fairphone";
const char* password = "TEMPPASS";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"(
<!DOCTYPE HTML><html>
<head>
  <title>AirGuard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>AirGuard dashboard </h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {

}


/*
#include <Arduino.h>
//Local libraries
#include "http.h"
#include "get_vals.h"
#include "start.h"
// External libraries
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


String PARAM_INPUT_1 = "output";
String PARAM_INPUT_2 = "state";

// Definitions

#define LED 2
String ssid = "Fairphone";
String password = "TEMPPASS";


// Variables
unsigned long lastTime = 0;
unsigned long timerDelay = 60000; // delay is 16 hours
JsonDocument doc;

String serverName = "https://www.hvakosterstrommen.no/api/v1/prices/2024/02-02_NO3.json"; // API endpoint

// DASHBOARD
AsyncWebServer server(80);


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
}
*/