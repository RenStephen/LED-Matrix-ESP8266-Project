#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <ESP8266WebServer.h>

#include "utils.h"
#include "Weather.h"
#include "matrix.h"

int state = 0;

void handleIndex()
{
  String path = "/test.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path))
  {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
  }
  else
  {
    server.send(INTERNAL_SERVER_ERROR, "text/plain", "500, no index file found");
  }
}

void setRandomGameState()
{
  game.setRandomGameState();
  state = 1;
  server.send(HTTP_OK, "text/plain", "matrix has been set!");
}

void handleRain()
{
  state = 3;
  server.send(HTTP_OK, "text/plain", "its raining now");
}

void handleWeather()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  WeatherClient client;
  if (!client.getWeatherData() || !client.parseContent())
  {
    server.send(INTERNAL_SERVER_ERROR, "test/plain", "500, looks like there was a problem with getting the weather data.");
    return;
  }
  Serial.println(client.getJsonContent());
  server.send(HTTP_OK, "text/plain", client.getTemperatureCelcius());
}

void handleNotFound()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(NOT_FOUND, "text/plain", "404 resource not found. try another endpoint like /weather, or /");
}

void setGameState()
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if (!server.hasArg("plain"))
  {
    server.send(HTTP_OK, "text/plain", "Body not received");
    return;
  }
  String message = "Body received:\n";
  message += server.arg("plain");
  message += "\n";
  Serial.println(message);
  server.send(HTTP_OK, "text/plain", message);
}

void setup()
{
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
  server.on("/handle_weather", handleWeather);
  server.on("/set_random_game_state", setRandomGameState);
  server.on("/set_game_state", setGameState);
  server.on("/handle_rain", handleRain);
  server.onNotFound(handleNotFound);
  server.begin(); //Start the servers
}

void loop()
{
  server.handleClient();
  displayMatrix(state);
}
