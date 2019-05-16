#include <WiFiManager.h>
// #include "config.h"

void wifiSetup(bool onDemand);

void wifiSetup(bool onDemand = false) {
  String mac = "<p>This devices MAC-Address is: ";
  mac += WiFi.macAddress();
  mac += "</p>";

  String fp_info = "<p>Run <pre>echo | openssl s_client -connect MYMQTTSERVER:8883 | openssl x509 -fingerprint -noout</pre> to get your TLS fingerprint.</p>";

  WiFiManagerParameter custom_text(mac.c_str());
  WiFiManagerParameter custom_text_ssl(fp_info.c_str());
  WiFiManagerParameter custom_mqtt_server("server", "MQTT Server IP", mqtt_server, 128);
  WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_user("user", "MQTT Username", mqtt_user, 128);
  WiFiManagerParameter custom_mqtt_password("password", "MQTT Password", mqtt_password, 128);
  WiFiManagerParameter custom_mqtt_topic("topic", "MQTT Topic", mqtt_topic, 128);
  WiFiManagerParameter custom_mqtt_fingerprint("fingerprint", "MQTT Fingerprint", mqtt_fingerprint, 128);

  WiFiManager wifiManager;
  wifiManager.setAPStaticIPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  wifiManager.setTimeout(240);

  wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_text);
  // wifiManager.addParameter(&custom_text_ssl);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_mqtt_topic);
  // wifiManager.addParameter(&custom_mqtt_fingerprint);

  if (onDemand) {
    wifiManager.startConfigPortal("Wake-on-ESP");
  } else {
    wifiManager.autoConnect("Wake-on-ESP");

  }
  
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_user, custom_mqtt_user.getValue());
  strcpy(mqtt_password, custom_mqtt_password.getValue());
  strcpy(mqtt_topic, custom_mqtt_topic.getValue());
  strcpy(mqtt_fingerprint, custom_mqtt_fingerprint.getValue());
}
