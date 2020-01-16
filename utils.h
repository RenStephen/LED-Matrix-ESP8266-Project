#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 

// the webserver. this hosts the api endpoitns
ESP8266WebServer server(80);

// WIFI configurations
const String NETWORK_SSID    = "NETGEAR49";
const String PASSWORD        = "calmink368";

//HTML Status codes
const int HTTP_OK               = 200;
const int NOT_FOUND             = 404;
const int INTERNAL_SERVER_ERROR = 500;
const int BAD_REQUEST           = 400;

void connectToWIFI() {
  // connect to the wifi network
  Serial.println("\n\nConnecting to: "+NETWORK_SSID+"\n");
  
  WiFi.begin(NETWORK_SSID, PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected\nIP address: ");  
  Serial.println(WiFi.localIP());
}

// convert the file extension to the MIME type
String getContentType(String filename) { 
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}
