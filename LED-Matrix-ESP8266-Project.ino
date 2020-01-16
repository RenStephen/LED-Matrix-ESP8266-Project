#include <ESP8266WiFi.h>
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
  GameOfLife game = GameOfLife();
  for (int j = 0; j < 100; j++) {
    for (int i = 0; i < SIZE; i++) {
      if (game.getCellState(i)) {
        matrix.drawPixel(0, i, matrix.Color((int)random(255), (int)random(255), (int)random(255)));
      } else {
        matrix.drawPixel(0, i, matrix.Color(0, 0, 0));
      }
    }
    matrix.show();
    delay(100);
    game.calculateNextState();
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

  randomSeed(analogRead(0)); 
  
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
