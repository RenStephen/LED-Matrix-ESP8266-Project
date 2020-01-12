#include <String.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Weather station configurations
const String CITY     = "Boston,US";
const String API_KEY  = "7a18c87bc7bb4ab790913c0b83fc4feb";
const String ERROR    = "error";

// the size of the weather api responses varies. this is a stable number
const size_t CAPACITY = 2048;

WiFiClient client;

class WeatherClient {
    private:
        String content;
        float tempC;
        String conditions;
    public:

        boolean parseContent() {
            DynamicJsonDocument doc(CAPACITY);
            DeserializationError error = deserializeJson(doc, this->content);
            if (error) {
                return false;
            }
            JsonObject list_0 = doc["list"][0];
            JsonObject list_0_main = list_0["main"];
            this->tempC = list_0_main["temp"];
            JsonObject list_0_weather_0 = list_0["weather"][0];
            const char* c = list_0_weather_0["main"];
            this->conditions = String(c);
            return true;
        }

        String getTemperatureCelcius() {
            return String(this->tempC);
        }
        String getTemperatureFahrenheit() {
            float tempF = (this->tempC * 1.8) + 32;
            return String(tempF);
        }
        String getConditions() {
            return this->conditions;
        }
        String getJsonContent() {
            return this->content;
        }
        boolean getWeatherData() {
            client.stop();
            if (client.connect("api.openweathermap.org", 80)) {
                client.println("GET /data/2.5/forecast?q=" + CITY + "&APPID=" + API_KEY + "&mode=json&units=metric&cnt=2 HTTP/1.1");
                client.println("Host: api.openweathermap.org");
                client.println("User-Agent: ArduinoWiFi/1.1");
                client.println("Connection: close");
                client.println();
            }

            unsigned long timeout = millis();
            while (client.available() == 0) {
                if (millis() - timeout > 10000) {
                    client.stop();
                    return false;
                }
            }

            while (client.connected() || client.available()) {
                if (client.available()){
                    if (client.find("\r\n\r\n")) {
                        break;
                    } else {
                        return false;
                    }
                }
            }
            this->content = client.readStringUntil('\n');            
            return true;
        }
};