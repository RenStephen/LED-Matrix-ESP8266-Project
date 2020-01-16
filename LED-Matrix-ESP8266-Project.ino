#include <ESP8266WiFi.h>
#include <HttpClient.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include "Game_of_Life.h"
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
  for (int i = 0 ; i < SIZE ; i++) {
    matrix.drawPixel(0, i, matrix.Color(255,0,0));
    delay(50);
    matrix.show();
  }
  server.send(HTTP_OK, "text/plain", "matrix has been set!");
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

  //setup the matrix
  setupMatrix();

  // start the file system
  SPIFFS.begin();

  // begin the server endpoints
  server.on("/", handleIndex);
  server.on("/handle_rain", handleRain);
  server.on("/set_matrix", setMatrix);
  server.onNotFound(handleNotFound);
  server.begin(); //Start the servers
  
}

void loop() {
  server.handleClient();
}
