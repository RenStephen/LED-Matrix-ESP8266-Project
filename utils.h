#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 

#define PIN D3

ESP8266WebServer server(80);
Adafruit_SSD1306 lcd(-1);

// WIFI configurations
const String NETWORK_SSID     = "NETGEAR49";
const String PASSWORD         = "calmink368";

// const String NETWORK_SSID     = "Linksys28761";
// const String PASSWORD         = "01vzecjkii";

//HTML Status codes
const int HTTP_OK               = 200;
const int NOT_FOUND             = 404;
const int INTERNAL_SERVER_ERROR = 500;
const int BAD_REQUEST           = 400;

void setupLCD() {
  lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  lcd.setTextColor(WHITE); 
  lcd.clearDisplay(); lcd.display(); 
}

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
