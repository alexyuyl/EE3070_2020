#include <WiFiEsp.h>
#include <ThingSpeak.h>

//wifi setting
char ssid[] = "ww";
char pass[] = "wififortest";
int status = WL_IDLE_STATUS;

//thingspeak channel setting
unsigned long CHID = 1297682;
const char* readkey = "TSE206UHIU99F4X4";
unsigned int Sfield = 1;
WiFiEspClient client;
int statuscode = 0;

void setup() {
  //set both baud rate
  Serial.begin(115200);
  Serial1.begin(115200);

  //wifi initialization
  WiFi.init(&Serial1);
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("n\Connected");
    printWifiData();
    ThingSpeak.begin(client);//thingspeak initialization
  }
}

void loop() {
  float field1 = ThingSpeak.readFloatField(CHID, Sfield,readkey);
  statuscode = ThingSpeak.getLastReadStatus();
  if (statuscode == 200){
     Serial.println(field1);
     Sfield = Sfield + 1;
     if (Sfield == 4){
       Sfield = 1;
     }
  }
  else Serial.println("failed to read");
  Serial.println("updating...");
  delay(10000);
}

void printWifiData(){
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
