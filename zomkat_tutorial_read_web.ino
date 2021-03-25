//zoomkat 5-13-13
//simple client test
//for use with IDE 1.0
//open serial monitor and send an e to test
//for use with W5100 based ethernet shields

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
char serverName[] = "web.comporium.net"; // zoomkat's test web page server
EthernetClient client;

String readString, readString1;
int x=0; //for counting line feeds
char lf=10; //line feed character
//////////////////////

void setup(){

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  Serial.begin(9600);
  Serial.println("Better client test 5/13/13"); // so I can keep track of what is loaded
  Serial.println("Send an e in serial monitor to test"); // what to do to test
}

void loop(){
  // check for serial input
  if (Serial.available() > 0) //if something in serial buffer
  {
    byte inChar; // sets inChar as a byte
    inChar = Serial.read(); //gets byte from buffer
    if(inChar == 'e') // checks to see byte is an e
    {
      sendGET(); // call sendGET function below when byte is an e
    }
  } 
}

//////////////////////////

void sendGET() //client function to send/receive GET request data.
{
  if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    client.println("GET /~shb/arduino.txt HTTP/1.1"); //download text
    client.println("Host: web.comporium.net");
    client.println("Connection: close");  //close 1.1 persistent connection 
    client.println(); //end of get request
  }
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints raw feed for testing
    if (c==lf) x=(x+1); //counting line feeds
    if (x==9) readString += c; //building readString
   }

  Serial.println(); 
  Serial.println();
  Serial.print("Current data row:" );
  Serial.print(readString); //the 10th line captured
  Serial.println();
  readString1 = (readString.substring(0,8)); //extracting "woohoo!"
  Serial.println();
  Serial.print("How we feeling?: ");
  Serial.println(readString1);
  Serial.println();     
  Serial.println("done");
  Serial.println("disconnecting.");
  Serial.println("==================");
  Serial.println();
  readString = ("");
  readString1 = (""); 
  client.stop(); //stop client
}
