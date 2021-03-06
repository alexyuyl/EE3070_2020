#include <WiFiEsp.h>
//#include <ThingSpeak.h>
#include "SPI.h"

char c;
char ssid[] = "ww";            // your network SSID (name)
char pass[] = "wififortest";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

String readString;

char server[] = "www.thingspeak.com";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200);
  delay(1000);
  // initialize ESP module
  WiFi.init(&Serial1);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  // you're connected now, so print out the data
  Serial.println("You're connected to the network");  
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    client.println("GET /channels/1324991/status/last HTTP/1.1"); 
    client.println("Host: https://www.thingspeak.com"); //https is used for secured website
    client.println("Connection: close");
    client.println();
  }
}

void loop()
{
  while (client.available()){  
    char c = client.read(); //gets byte from ethernet buffer
    Serial./*println*/write(c);
    if ((c <= 9 && c >= 0)|| c == 'T' || c == 'Z' || c == "-"){
      readString += c;
    }
    }
 /*client.stop(); //stop client
  Serial.println("client disconnected.");
  Serial.println("Data from server captured in readString:");
  Serial.println();
  Serial.print(readString); //prints readString to serial monitor
  Serial.println();  
  Serial.println();
  Serial.println("End of readString");
  Serial.println("==================");
  Serial.println();
  delay(10000);*/

}



void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
