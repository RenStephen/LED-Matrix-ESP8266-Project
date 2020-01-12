#include <ESP8266WiFi.h>
#include <HttpClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <FS.h>

#include "utils.h"
#include "Weather.h"

void handleIndex(){
  String path = "/test.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");  
    size_t sent = server.streamFile(file, contentType);
    file.close();                                       
  } else {
    server.send(INTERNAL_SERVER_ERROR, "text/plain", "500, no index file found");
  }
}

void setMatrix() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if (!server.hasArg("payload")) {
    server.send(BAD_REQUEST, "test/plain", "BAD REQUEST");
    return;
  }
  String payload = server.arg("payload");
  // here i need to parse the payload delimmit on strings. we are expecting a boolean or byte array

}

void handleRain() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  WeatherClient client;
  if (!client.getWeatherData() || !client.parseContent()) {
    server.send(INTERNAL_SERVER_ERROR, "test/plain", "500, looks like there was a problem with getting the weather data.");
    return;
  }
  Serial.println(client.getJsonContent());
  server.send(HTTP_OK, "text/plain", client.getTemperatureCelcius());
}

void handleNotFound() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(NOT_FOUND, "text/plain", "404 resource not found. try another endpoint like /weather, or /");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // connect to the wifi network
  connectToWIFI();

  // start the file system
  SPIFFS.begin();

  // begin the server endpoints
  server.on("/", handleIndex);
  server.on("/handle_rain", handleRain);
  server.onNotFound(handleNotFound);
  server.begin(); //Start the servers
}

void loop() {
  server.handleClient();
}
