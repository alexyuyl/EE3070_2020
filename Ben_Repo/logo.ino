#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char logo_bmp [] PROGMEM = { //https://www.pngkey.com/png/detail/236-2369212_this-icon-is-a-continuous-loop-like-the.png
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x00, 0x0f, 0xc0, 0x00, 0x3f, 0xfc, 0x00, 0x03, 0xf0, 0x00, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x7f, 0xf8, 0x00, 0x1f, 0xf8, 0x00, 0x1f, 0xfe, 0x00, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0x00, 0x0f, 0xf0, 0x00, 0x7f, 0xff, 0x80, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0x80, 0x07, 0xe0, 0x01, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xc0, 0x03, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x01, 0xc0, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x80, 0x1f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x7f, 0xf8, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xf8, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xfc, 0x7f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0xff, 0xfc, 0x7f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xe0, 0x03, 0xff, 0x80, 0x01, 0xff, 0xc0, 0x07, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x7f, 0xe0, 0x01, 0xff, 0xc0, 0x03, 0xff, 0xc0, 0x07, 0xfe, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x7f, 0xe0, 0x01, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x07, 0xfe, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x7f, 0xe0, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x07, 0xfe, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xf8, 0x3f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xfc, 0x7f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xfc, 0x7f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xfc, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x01, 0x80, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xe0, 0x03, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x01, 0xff, 0xff, 0x00, 0x07, 0xe0, 0x00, 0xff, 0xff, 0x80, 0x0f, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0xff, 0xfc, 0x00, 0x0f, 0xf0, 0x00, 0x3f, 0xff, 0x00, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x00, 0x3f, 0xf0, 0x00, 0x1f, 0xf8, 0x00, 0x0f, 0xfc, 0x00, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};//https://diyusthad.com/image2cpp


void setup() {
Serial.begin(115200);
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  delay(2000); // Pause for 2 seconds
display.clearDisplay();

//bitmap
display.drawBitmap(0, 0, logo_bmp, 128, 50, WHITE);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,51);
display.println("Alex    Jimmy     Ben");

display.display();
delay(4000); //can be 1s if needed
}

void loop() {
}
