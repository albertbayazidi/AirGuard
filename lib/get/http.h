#include <HTTPClient.h>
#include <Adafruit_BMP280.h>

void respCode(int httpResponseCode);

void sendresp(WiFiClient client);

void startDoc(WiFiClient client);

String outputState(int output);

void webPage(WiFiClient client,sensors_event_t temp_event,sensors_event_t pressure_event,float COData,String pris);
