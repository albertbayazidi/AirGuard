#include "http.h"

void respCode(int httpResponseCode){
    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void webPage(WiFiClient client,sensors_event_t temp_event,sensors_event_t pressure_event,float COData,String pris){
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>AirGuard</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>");
  client.println("/* Your CSS styles here */");
  client.println("body {");
  client.println("  background-color: #2ddbcc; /* Cool background color */");
  client.println("  font-family: Arial, sans-serif;");
  client.println("}");
  client.println("header {");
  client.println("  background-color: #333; /* Dark background color for header */");
  client.println("  color: #fff; /* Light text color */");
  client.println("  padding: 10px 0;");
  client.println("  text-align: center;");
  client.println("}");
  client.println(".tabs {");
  client.println("  margin: 20px 0;");
  client.println("  text-align: center;");
  client.println("}");
  client.println(".tab {");
  client.println("  display: inline-block;");
  client.println("  background-color: #666; /* Dark background color for tabs */");
  client.println("  color: #fff; /* Light text color */");
  client.println("  padding: 10px 20px;");
  client.println("  border-radius: 5px 5px 0 0;");
  client.println("  cursor: pointer;");
  client.println("  transition: background-color 0.3s;");
  client.println("}");
  client.println(".tab:hover {");
  client.println("  background-color: #555; /* Darker background color on hover */");
  client.println("}");
  client.println(".tab-content {");
  client.println("  display: none;");
  client.println("  padding: 20px;");
  client.println("  background-color: #fff; /* Light background color for tab content */");
  client.println("  border: 1px solid #ccc;");
  client.println("  border-top: none;");
  client.println("  border-radius: 25px;");
  client.println("  align-items: center;");
  client.println("  text-align: center;");
  client.println("}");
  client.println(".tab-content.active {");
  client.println("  display: block;");
  client.println("  align-items: center;");
  client.println("  text-align: center;");
  client.println("  background-color: darkcyan;");
  client.println("}");
  client.println(".box {");
  client.println("  width: 150px;");
  client.println("  height: 40px;");
  client.println("  vertical-align: top;");
  client.println("  border: 1px solid #ccc;");
  client.println("  border-radius: 25px;");
  client.println("  margin: 10px;");
  client.println("  padding: 10px;");
  client.println("  display: inline-block;");
  client.println("  text-align: center;");
  client.println("  color: #fff;");
  client.println("}");
  client.println(".background-image {"); 
  client.println("  position: fixed;"); 
  client.println("  bottom: 0;"); 
  client.println("  left: 0;"); 
  client.println("  width: 100%;"); 
  client.println("  height: auto; /* Adjust height as needed */"); 
  client.println("  z-index: -1; /* Ensure it's behind other content */"); 
  client.println("}"); 
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<header>");
  client.println("<h1>AirGuard Dashboard</h1>");
  client.println("<p>By Floorlabs</p>");
  client.println("</header>");
  client.println("<div class=\"tabs\">");
  client.println("<div class=\"tab\" onclick=\"showTab('room1')\">Room 1</div>");
  client.println("<div class=\"tab\" onclick=\"showTab('room2')\">Room 2</div>");
  client.println("<div class=\"tab\" onclick=\"showTab('room3')\">Room 3</div>");
  client.println("<!-- Add more tabs for additional rooms -->");
  client.println("</div>");
  client.println("<section id=\"room1\" class=\"tab-content\">");
  client.println("<!-- Display data for Room 1 -->");
  client.println("<div class=\"box\">Temperature: <span id=\"temp\">--</span> °C</div>");
  client.println("<div class=\"box\">CO2 Levels: <span id=\"co2\">--</span> ppm</div>");
  client.println("<div class=\"box\">Humidity: <span id=\"humidity\">--</span>%</div>");
  client.println("<div class=\"box\">Pressure: <span id=\"pressure\">--</span> hPa</div>");
  client.println("<div class=\"box\">price: <span id=\"price\">--</span> nok</div>");
  client.println("</section>");
  client.println("<section id=\"room2\" class=\"tab-content\">");
  client.println("<!-- Display data for Room 2 -->");
  client.println("<div class=\"box\">Data for Room 2</div>");
  client.println("</section>");
  client.println("<section id=\"room3\" class=\"tab-content\">");
  client.println("<!-- Display data for Room 3 -->");
  client.println("<div class=\"box\">Data for Room 3</div>");
  client.println("</section>");
  client.println("<img class=\"background-image\" src=\"https://raw.githubusercontent.com/albertbayazidi/AirGuard/main/img/bakgrunn.png\" alt=\"Background Image\">");
  client.println("<!-- JavaScript code for tab functionality and updating data -->");
  client.println("<script>");
  client.println("function showTab(tabId) {");
  client.println("  var tabs = document.querySelectorAll('.tab-content');");
  client.println("  tabs.forEach(function(tab) {");
  client.println("    tab.classList.remove('active');");
  client.println("  });");
  client.println("  document.getElementById(tabId).classList.add('active');");
  client.println("}");
  client.println("");
  client.println("// Function to update data (replace with actual data)");
  client.println("function updateData() {");
  client.println("  document.getElementById(\"temp\").innerText = " + String(temp_event.temperature) +";");
  client.println("  document.getElementById(\"co2\").innerText = " + String(COData) +";");
  client.println("  document.getElementById(\"humidity\").innerText = " + String(50) + ";");
  client.println("  document.getElementById(\"pressure\").innerText = " + String(pressure_event.pressure) +";");
  client.println("  document.getElementById(\"price\").innerText =" + String(pris) + ";");
  client.println("}");
  client.println("");
  client.println("// Helper function to get random number within a range");
  client.println("function getRandomNumber(min, max) {");
  client.println("  return Math.floor(Math.random() * (max - min + 1)) + min;");
  client.println("}");
  client.println("");
  client.println("// Call the updateData function when the page loads to initially populate the data");
  client.println("window.onload = updateData;");
  client.println("</script>");
  client.println("</body>");
  client.println("</html>");
}