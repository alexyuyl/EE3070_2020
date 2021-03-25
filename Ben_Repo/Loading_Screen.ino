#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SQUARESIZE 20

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
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

}
	int xpos = 0;
	int moving = 18;
	int mode = 0;
void loop() {
	display.clearDisplay();	
	//square 20X20
  display.fillRect(xpos, 22, SQUARESIZE, SQUARESIZE, SSD1306_WHITE);
	display.display();
	delay(1000);

	//xpos = (xpos + moving) % (SCREEN_WIDTH - SQUARESIZE); //from left to right	
	//mode = xpos >= (SCREEN_WIDTH - SQUARESIZE) ? 1 : 0;
	
	if (!mode) 
		xpos += moving;
	else xpos -= moving;
	if (xpos >= 108)
		mode = 1;
	if (xpos <18)
		mode = 0;
	
}
