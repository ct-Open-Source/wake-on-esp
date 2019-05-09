#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>

static AsyncUDP udp;
static uint16_t localUdpPort = 9;  // local port to listen on
static unsigned char incomingPacket[255];  // buffer for incoming packets
const byte preamble[] = {255,255,255,255,255,255};
static byte target_mac[6];

void udpCallback(AsyncUDPPacket packet);
void wolSetup();

void udpCallback(AsyncUDPPacket packet) {
  size_t len = packet.length();
  memcpy(&incomingPacket, packet.data(),len);

  if (len == 102) {

    Serial.printf("*WOL: Received a package from %s\n", packet.remoteIP().toString().c_str());
    if (len > 0) {
      incomingPacket[len] = 0;
    }

    bool packageIsValid = true;
    for(byte b=0; b<6; b++) {
        if(preamble[b] != incomingPacket[b])
        {
            packageIsValid = false;
            break;
        }
    }

    if(packageIsValid) {
          Serial.println("*WOL: Package seems to be a valid WOL-package");
    }

    for(byte b=6; b<12; b++) {
        if(int(target_mac[b-6]) != int(incomingPacket[b])) 
        {
            packageIsValid = false;
            break;
        }
    }

    if(packageIsValid) {
        Serial.println("*WOL: Package is destined for me.");
        action_power= 1;
    }

  }
}

void wolSetup() {
  Serial.print("*WOL: My MAC-Address is: ");
  String mac = WiFi.macAddress();
  Serial.println(mac);
  mac.replace(":","");
  StringToBytes(mac, target_mac); 
  udp.listen(localUdpPort);
  udp.onPacket(udpCallback);
  Serial.printf("*WOL: Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}
