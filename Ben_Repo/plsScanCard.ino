#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DELAYTIME 500 //0.5s
                      //at min can be 0.1s
#define SQUARESIZE 20


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup(){
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  
  plsScanCard();
}

void loop(){
}

void plsScanCard(){
  display.clearDisplay(); //init OLED display manual
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,32);
  display.println("Pls Scan Your Card");
  display.display();
  delay(DELAYTIME);
  display.startscrolldiagright(0x00, 0x07);
  delay(DELAYTIME);
  display.startscrolldiagleft(0x00, 0x07);
  delay(DELAYTIME);
}
