#include <WiFiEsp.h>
#include <ThingSpeak.h>
#include <Adafruit_SSD1306.h>

//wifi setting
char ssid[] = "ww2";
char pass[] = "wwwwwwww";
int status = WL_IDLE_STATUS;
WiFiEspClient client;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DELAYTIME 500 //0.5s
                      //at min can be 0.1s
#define SQUARESIZE 16

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long OnD = 1354115;
char WriteKey[] = "FMD4UE353KCAT4CM";

void setup() {
  //set both baud rate
  Serial.begin(115200);
  Serial1.begin(115200);

  display.display();
  //wifi initialization
  WiFi.init(&Serial1);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("n\Connected");
    ThingSpeak.begin(client);//thingspeak initialization
  }
  int xpos = 0;
  
  Goods_1_default_discount();
  for (int i = 0; i < 10; i ++){  
    loadingScreen(xpos);
    delay(DELAYTIME);
  }
  Goods_2_default_discount();
}
void loop() {
  
}

void Goods_1_default_stocking(){
  int x = ThingSpeak.writeField(OnD, 1, 10, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void Goods_1_default_price(){
  int x = ThingSpeak.writeField(OnD, 2, 50, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void Goods_2_default_stocking(){
  int x = ThingSpeak.writeField(OnD, 3, 10, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void Goods_2_default_price(){
  int x = ThingSpeak.writeField(OnD, 4, 50, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void Goods_1_default_discount(){
  int x = ThingSpeak.writeField(OnD, 6, 20, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void Goods_2_default_discount(){
  int x = ThingSpeak.writeField(OnD, 7, 20, WriteKey);
   if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void loadingScreen(int & xpos){
  int moving = 18;
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Loading");

  for(int i = 0; i < xpos; i+=moving)
    display.fillRect(i, 30, SQUARESIZE, SQUARESIZE, SSD1306_WHITE);
  display.display();
  
  xpos = (xpos + moving) % (SCREEN_WIDTH - SQUARESIZE + moving);
}
