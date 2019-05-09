#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>

static bool action_power = 0;
static bool action_power_force = 0;
static bool action_reset = 0;

static AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

void mqttSetup();

void connectToMqtt() {
  Serial.println("*MQTT: Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("*MQTT: Connected to MQTT.");
  Serial.print("*MQTT: Session present: ");
  Serial.println(sessionPresent);
  if (strlen(mqtt_topic)==0) {
    strcpy(mqtt_topic,"wakeonesp/wake");
  };
  uint16_t packetIdSub = mqttClient.subscribe(mqtt_topic, 2);
  Serial.print("*MQTT: Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("*MQTT: Disconnected from MQTT.");

  // if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) {
  //   Serial.println("Bad server fingerprint.");
  // }

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(5, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("*MQTT: Subscribe acknowledged.");
  Serial.print("*MQTT:   packetId: ");
  Serial.println(packetId);
  Serial.print("*MQTT:   qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("*MQTT: Unsubscribe acknowledged.");
  Serial.print("*MQTT:   packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("*MQTT: Publish received.");
  Serial.print("*MQTT:   topic: ");
  Serial.println(topic);
  Serial.print("*MQTT:   qos: ");
  Serial.println(properties.qos);
  Serial.print("*MQTT:   dup: ");
  Serial.println(properties.dup);
  Serial.print("*MQTT:   retain: ");
  Serial.println(properties.retain);
  Serial.print("*MQTT:   len: ");
  Serial.println(len);
  Serial.print("*MQTT:   index: ");
  Serial.println(index);
  Serial.print("*MQTT:   total: ");
  Serial.println(total);
  Serial.println();
  if (!strncmp(payload, "on", len)) {
    action_power = 1;
  } else if (!strncmp(payload, "force_off", len)) {
    action_power_force = 1;
  } else if (!strncmp(payload, "reset", len)) {
    action_reset = 1;
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("*MQTT: Publish acknowledged.");
  Serial.print("*MQTT:   packetId: ");
  Serial.println(packetId);
}

void mqttSetup() {
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  IPAddress mqtt_ip;
  mqtt_ip.fromString(mqtt_server);

  Serial.print("*MQTT: Connecting to: ");
  Serial.println(mqtt_ip);

  mqttClient.setServer(mqtt_ip, atoi(mqtt_port));
  mqttClient.setCredentials(mqtt_user, mqtt_password);
  String clientIdStr = "WoE-"+WiFi.macAddress();
  char clientId[24];
  clientIdStr.toCharArray(clientId,24);
  mqttClient.setClientId(clientId);

  // if (strncmp(mqtt_port, "8883", 4)) {
  //   mqttClient.setSecure(true);
  //   byte target_fp[128];
  //   StringToBytes((String)mqtt_fingerprint, target_fp);
  //   mqttClient.addServerFingerprint(target_fp);
  // }
  mqttClient.connect();
}
