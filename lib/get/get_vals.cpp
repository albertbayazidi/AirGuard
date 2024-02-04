#include "get_vals.h"

void printVals(JsonDocument doc,String valName){
    JsonArray array = doc.as<JsonArray>();
    for (JsonObject num : array) {
        Serial.print(valName + ": ");
        Serial.println(num[valName].as<double>(),5);
    }
    
}

String avgVals(JsonDocument doc,String valName){
    float sum = 0;
    int count = 0;
    JsonArray array = doc.as<JsonArray>();
    for (JsonObject num : array) {
        sum += num[valName].as<double>();
        count++;
    }
    return String(sum/count);
}
