#include <WiFiEsp.h>

char ssid[] = "EE3070_P1800";
char pass[] = "EE3070P1800";
int status = WL_IDLE_STATUS;

void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }
}

void loop(){
  Serial.println();
  printWifiData();
  delay(10000);
}

void printWifiData(){
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
