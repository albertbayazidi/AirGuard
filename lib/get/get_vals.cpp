#include "get_vals.h"

void printVals(JsonDocument doc,String valName){
    JsonArray array = doc.as<JsonArray>();
    for (JsonObject num : array) {
        Serial.print(valName + ": ");
        Serial.println(num[valName].as<double>(),5);
    }
    
}