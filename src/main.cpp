#include <FS.h> 
#include <Arduino.h> 
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "config.h"
#include "hextools.h"
#include "wifi.h"
#include "pin.h"
#include "mqtt.h"
#include "wol.h"

void setup() {
  Serial.begin(115200);
  Serial.println("===Wake-on-ESP===");
  loadConfig();

  wifiSetup();

  if (shouldSaveConfig) {
    saveConfig();
  }

  pinSetup();
  wolSetup();
  mqttSetup();
}

void loop() {
  if(action_power == 1){
    pinToggle(POWER_PIN, 200);
    action_power = 0;
  }
  if(action_power_force == 1){
    pinToggle(POWER_PIN, 5000);
    action_power_force = 0;
  }
  if(action_reset == 1){
    pinToggle(RESET_PIN, 200);
    action_reset = 0;
  }

   // is configuration portal requested?
  if ( digitalRead(CONFIG_PIN) == LOW ) {
    wifiSetup(true);
  }
  if (shouldSaveConfig) {
    saveConfig();
  }
}
