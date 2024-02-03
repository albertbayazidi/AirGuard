#include "start.h"


void startWifi(String ssid,String password){
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    WiFi.mode(WIFI_STA);
    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
} 

