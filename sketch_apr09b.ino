#include <WiFiEsp.h>
//#include <ThingSpeak.h>
#include "SPI.h"

char c;
char ssid[] = "ww2";            // your network SSID (name)
char pass[] = "wwwwwwww";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

String readString;
String readString2;

String utcTime, utcYear, utcMonth, utcDay, utcHour;
int localMonth, localDay, localHour, localMinute, localSecond;
String localTime;
String temp1, temp2;
String newlocalHour = "0";
String newlocalDay = "0";
String newlocalMonth = "0";
String newlocalMinute = "0";
String newlocalSecond = "0";
int type;

bool done = 0;

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
    //Serial./*println*/write(c);
    readString += c;
    done = 1;
    }
    if (done == 1){
      readString2 = readString.substring(649,700);
      Serial.println(readString2);
      utctolocal();
      done = 0;
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

void utctolocal()
{
  utcTime = readString2.substring(15,35);
  utcYear = utcTime.substring(0,4);
  utcMonth = utcTime.substring(5,7);
  utcDay = utcTime.substring(9,10);
  utcHour = utcTime.substring(12,13);
  temp1 = utcTime.substring(14,16);
  temp2 = utcTime.substring(17,19);
  localMinute = temp1.toInt();
  localSecond = temp2.toInt();
  
  //get local time from current utc time
  localHour = utcHour.toInt() + 8;
  //if utc hour == 4pm or above, local time would be in the next day, therefore need to adjust date
  if (localHour >= 24){
    localHour -= 24;
    localDay = utcDay.toInt() + 1;
  }
  else localDay = utcDay.toInt();
  localMonth = utcMonth.toInt();
  if (localMonth == 1||localMonth == 3||localMonth == 5||localMonth == 7||localMonth == 8||localMonth == 10||localMonth == 12)
    type = 1;
  else if (localMonth == 2)
    type = 3;
  else type = 2;
  if (type == 1 && localDay > 31){
    localDay = 1;
    localMonth ++;
  }
  else if (type == 2 && localDay > 30){
    localDay = 1;
    localMonth ++;
  }
  else if (type == 3&& localDay > 28){
    localDay = 1;
    localMonth ++;
  }
  if (localSecond <= 9)
    newlocalSecond += localSecond;
  else newlocalSecond = localSecond;
  if (localMinute <= 9)
    newlocalMinute += localMinute;
  else newlocalMinute = localMinute;
  if (localHour <= 9)
    newlocalHour += localHour;
  else newlocalHour = localHour;
  if (localDay <= 9)
    newlocalDay += localDay;
  else newlocalDay = localDay;
  if (localMonth <= 9)
    newlocalMonth += localMonth;
  else newlocalMonth = localMonth;
  
  //replace utc time with local time
  localTime = utcYear + "-" + newlocalMonth + "-" + newlocalDay + "T" + newlocalHour + ":" + newlocalMinute + ":" + newlocalSecond + "Z";
  Serial.println(localTime);
}
