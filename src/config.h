#include <FS.h>
#include <ArduinoJson.h>

static char mqtt_server[128] = ""; 
static char mqtt_port[6] = ""; 
static char mqtt_user[128] = "";
static char mqtt_password[128] = "";
static char mqtt_topic[128] = "";
static char mqtt_fingerprint[256] = "";

static bool shouldSaveConfig = false;

void saveConfigCallback();
void loadConfig();
void saveConfig();

void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void loadConfig() {
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, buf.get());
        if (error)
            Serial.println("Failed to read file, using default configuration");

        JsonObject json = doc.as<JsonObject>();
        if (!json.isNull()) {
          Serial.println("Parsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_password, json["mqtt_password"]);
          strcpy(mqtt_topic, json["mqtt_topic"]);
          strcpy(mqtt_fingerprint, json["mqtt_fingerprint"]);

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }  
  } else {
    Serial.println("Failed to mount FS. Formatting in 10 Seconds.");
    delay(10000);
    SPIFFS.format();
  }
}

void saveConfig() {
  Serial.println("saving config");

  StaticJsonDocument<1024> doc;

  doc["mqtt_server"] = mqtt_server;
  doc["mqtt_port"] = mqtt_port;
  doc["mqtt_user"] = mqtt_user;
  doc["mqtt_password"] = mqtt_password;
  doc["mqtt_topic"] = mqtt_topic;
  doc["mqtt_fingerprint"] = mqtt_fingerprint;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
  shouldSaveConfig = false;
}
