#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

int heat = 4; // monoxide heater


void setup() {
  Serial.begin(115200);
  bmp.begin();
  aht.begin();
  
  pinMode(heat, OUTPUT);
}

void loop() {

  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  
  
  Serial.println(temp_event.temperature);
  Serial.println(temp.temperature);
  Serial.println(pressure_event.pressure);
  Serial.println(humidity.relative_humidity);

  Serial.println();

  
  analogWrite(heat, 170); // set heating element i monoxide at 1.5v
  Serial.println(analogRead(34)); // no unit what unit is, do some nice map() functions
  Serial.println();
  Serial.println();
  delay(2000);
}
