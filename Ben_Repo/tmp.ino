#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DELAYTIME 500 //need further testing

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//int manual_dig;   //user input, digit reed by serial.read
//int manual_dig_cb; //for storing the dummy enter key

void setup() { 
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial default display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  //init OLED ended

}

void loop() {

  mainMenu(); //plain display function for main menu
  int manualDig = checkSerial(); //user input 
  plsScanCard(); //display function fro pls scan card
  bool cardCheck = checkCardScan(1); //function check if card is scanned

  switch(manualDig)
  {
    case 1:
      //check whether the card is a user card
      //return cardCheck = true if it is a user card, vice versa
      if(cardCheck) 
        shopMode(); //customer page, shopping mode enabled
      break;
    case 2:
      //check whether the card is an employee card
      //return cardCheck = true if it is a employee card, vice versa
      if(cardCheck)
        empMode(); //employee page, employee mode enabled 
      break;
    case 3:
      if(cardCheck())
        writeEmp();
      break;
    case 4:
      if(cardCheck())
        writeProduct(); //Write The default Product Info Into The Card/Tag
      break;
    default:
      //error page
  }
  
}

//display function for main menu
void mainMenu(){ 
  display.clearDisplay(); //init OLED display manual
  display.setTextSize(2);
  display.setTextColor(WHITE);      //sample output
  display.setCursor(0,0);           //Press Key
  display.println("Press Key");     //-------------
  display.setCursor(0,20);          //1:Customer
  display.println("1:Customer");    //2:Employee
  display.setCursor(0,36);
  display.println("2:Employee");
  display.drawLine(0,16,128,16,WHITE);
  display.display();
  delay(DELAYTIME);
}

//display pls scan card
void plsScanCard(){ 
  display.clearDisplay(); //init OLED display manual
  display.setTextSize(1);
  display.setCursor(0,32);
  display.println("Pls Scan Your Card");
  display.display();
  display.startscrolldiagright(0x00, 0x0F);
  delay(DELAYTIME);
}

bool checkCardScan(int mode)
{ 
  
  // mode 1 check whether is user card 
  // mode 2 check whether is product tag

  bool success = true;

  if ( ! rfid.PICC_IsNewCardPresent()) // Check whether there is PICC card
      success = false;;
  if ( ! rfid.PICC_ReadCardSerial()) //Read the card, if true, then continue
      success = false;;  
            
  MFRC522::PICC_Type piccType;
  piccType = rfid.PICC_GetType(rfid.uid.sak);
    
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
      {
       success = false;;
      }

  byte cardInfo[18];
  readBlock(1, cardInfo);

  // check whether is a user card
  if(mode == 1)
  {
    
    if(cardInfo[0] != 1){
      // print "Not a user card"
      success = false;;
    }

  }
  else if(mode == 2)
  {
    if(cardInfo[0] != 0 || cardInfo[15] != 0){
      // print "Not a product tag"
      success = false;;
    }

  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  return success;
}


void writeEmp(){ //Write The default EMP Info Into The Card/Tag
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println("Write The default EMP Info Into The Card/Tag");
  display.display();
  delay(DELAYTIME);
}

void writeProduct(){ //Write The default Product Info Into The Card/Tag
  display.clearDisplay();
  display.setCursor(0,0); 
  display.println("Write The default Product Info Into The Card/Tag");
  display.display();
  delay(DELAYTIME);
}

void shopMode(){ //print_shopping_manual
  display.clearDisplay();//init OLED for text output
  display.setTextSize(1);
  display.setTextColor(WHITE);
  bool terminate = false;
  while(!terminate)
  {
    display.setCursor(0,0);
    display.println("Customer Manual:");
    display.setCursor(0,8);
    display.println("1:Show Product Info");
    display.setCursor(0,16);
    display.println("2:Take Consumption");
    display.setCursor(0,24);
    display.println("3:Leave Manual");
    display.display();
    delay(DELAYTIME);//may need testing

    int manualDig = checkSerial();

    switch(manualDig)
    {
      case 1: //show product info
              shProduct();
              if(back_exit_manual() == 4)
                return;
              break;
            
      case 2: //take consumption
              takeConsumption();
              if(back_exit_manual() == 4)
                return;
              break;
                

      case 3: //leave manual
              terminate  = true;
              exiting();
              return;
      default:
            //error page
    }
  }
}

void shProduct(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Please Scanned The Product Tags To Show The Info ");
  display.setCursor(0,16);
  display.println("Enter 5 to finish scanning");
  display.display();
  //delay(DELAYTIME); 
  int userMoney  = 0;
  scanProduct(1,userMoney);
}

void scanProdcut(int mode, int& userMoney)
{

  //mode 1 for viewing product info, will ignore userMoney
  //mode 2 for purchasing 

  while(1)
  {

    if(Serial.available()) 
    {
      String inp = Serial.readString();
      int manualDig =  atoi(inp.c_str());
      if(manualDig == 5)
      {
        return;
      }
      else
      {
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Invalid Input");
        display.setCursor(0,8);
        display.println("Enter 5 to finish");
        display.setCursor(0,16);
        display.println("Next product to scan");
        display.display();
        delay(DELAYTIME);
      }
        
    }

    //  check product tag valid or not
    bool cardcheck = checkCardScan(2);  

    if(cardcheck == true)
    {
      if(mode == 1)
      {
        productInfo();
      }
      else if(mode == 2)
      {
        purchase(userMoney);
        if(userMoney < 0)
          return;
      }
      //print product info
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Next product to scan");
      display.display();
      delay(DELAYTIME);
      //print "Next product to scan" 
    }
    else
    {
      //print "Invalid Product Tag" 
      //print "Next product to scan"
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Invalid Product Tag");
      display.setCursor(0,8);
      display.println("Next Product to scan");
      display.display();
      delay(DELAYTIME);
    }
  }
}

void takeConsumption(){

  //scan user card first

  int userMoney;

   if(checkCardScan())
  {
    userMoney = userInfo();
  }
  else
  {
    // print invalid card
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Invalid Card");
    display.display();
    delay(DELAYTIME);
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Please Scanned The Product Tags To Show The Info ");
  display.display();
  display.startscrolldiagright(0x00, 0x0F);
  delay(DELAYTIME);

  scanProdcut(2,userMoney);

  byte writeVal[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  if(checkCardScan())
  {
    store_dec(writeVal,userMoney,0);
    writeBlock(18,writeVal);
    ThingSpeak.writeField(CHID, 1, userMoney, WriteKey);
  }
  else
  {
    //print invalid card
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Invalid Card");
    display.display();
    delay(DELAYTIME);
    return;
  }
}

void empMode(){
    display.clearDisplay();//init OLED for text output
    display.setTextSize(1);
    display.setTextColor(WHITE);
    bool terminate = false;
  while(!terminate)
  {
    display.setCursor(0,0);
    display.println("Employee Manual:");
    display.setCursor(0,8);
    display.println("1:Show Personal Info");
    display.setCursor(0,16);
    display.println("2:Take Attendence");
    display.setCursor(0,24);
    display.println("3:View Product Info");
    display.setCursor(0,32);
    display.println("4:Leave Manual");
    display.display();
    delay(DELAYTIME);
    int manualDig = checkSerial();   

    switch(manual_dig)
    {
      case 1: //show emp personal info
              empInfo();
              if(back_exit_manual() == 4)
                return;
              break;

      case 2: //take attendence
              takeAttence();
              if(back_exit_manual() == 4)
                return;
              break;

      case 3: //viewing all product info
              break;
      case 4: //leave manual
              terminate = true;
              exiting();
              break;
      default:
              //error page

    }
    
  }
}

int back_exit_manual(){
  while(1)
  {
    int monitor_receive = checkSerial();
    switch(monitor_receive)
    {
      case 3:
            return 3;
      case 4:
            return 4;
      default:
             Serial.println("Invalid Input! Please Enter Again.");
             break;
    }
  }
}

void purchase(int& userMoney)
{
  productInfo();
  byte read_back_array[18];
  readBlock(14,read_back_array);
  int price = numeric_decode(read_back_array);
  userMoney = userMoney - price;
  
  if(userMoney < 0)
  {
    //print not enough money
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("not enough money!");
    display.setCursor(0,8);
    display.println("not enough money!");
    display.setCursor(0,16);
    display.println("not enough money!");
    display.setCursor(0,24);
    display.println("not enough money!");
    display.setCursor(0,32);
    display.println("not enough money!");
    display.display();
    delay(DELAYTIME);
    int xpos = 0;
    for(int i =0; i<10; i++)
    {
      exiting(xpos);
      delay(DELAYTIME/5);
    }
    return;
  }

  readBlock(18,read_back_array);
  int consumTimes = numeric_decode(read_back_array);
  consumTimes = consumTimes + 1;

  byte default_val[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  store_dec(default_val,consumTimes,0);
  writeBlock(18,default_val);
  ThingSpeak.writeField(CHID, 2, temp_re, WriteKey);

}

void productInfo()
{

  byte read_back_array[18];

  display.clearDisplay(); //init OLED display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  Serial.println(F("\nThe Product Name is : "));
  readBlock(6,read_back_array);
  String tmp = string_decode(read_back_array);
  Serial.print(tmp);
  display.println("Name:");
  display.setCursor(0,8);
  display.println(tmp);
    
  Serial.print(F("\nThe Product Type is :"));
  readBlock(10,read_back_array);
  tmp = string_decode(read_back_array);
  Serial.print(tmp);
  display.setCursor(0,16);
  display.println(tmp);

  Serial.println(F("\nThe Price Of The Product : "));
  readBlock(14,read_back_array);
  tmp = numeric_decode(read_back_array);
  Serial.print(tmp);
  display.setCursor(0,24);
  display.println("Price($) :");
  display.setCursor(0,32);
  display.println(tmp);
  display.display();

  Serial.println(F("\nThe Product has been consumed for : "));
  display.setCursor(0,40);
  display.println("Consumed for");
  readBlock(18,read_back_array);
  Serial.print(numeric_decode(read_back_array));
  display.setCursor(0,48);
  display.println(numeric_decode(read_back_array));
  Serial.print(F(" times."));

}



void empInfo(){ //Print emp personal info
  
  byte read_back_array[18];

  display.clearDisplay(); //init OLED display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  readBlock(6,read_back_array); //emp name
  String tmp = string_decode(read_back_array);
  display.println("Name:");
  display.setCursor(0,8);
  display.println(tmp);

  readBlock(10,read_back_array); //emp job
  tmp = string_decode(read_back_array);
  display.setCursor(0,16);
  display.println(tmp);

  readBlock(14,read_back_array); //salary
  display.setCursor(0,24);
  display.println("Salary($) is :");
  display.setCursor(0,32);
  display.println(numeric_decode(read_back_array));
  display.display();
  delay(DELAYTIME);

  monitor_check(manual_dig);
  display.setCursor(0,40);
  display.println("Press 5 to back");
  display.setCursor(0,48);
  display.println("Press 4 to exit");
  display.display();
  delay(DELAYTIME);
  monitor_check(manual_dig);
}

void takeAttence(){ //take attence function for emp
  //internet

  display.clearDisplay(); //init OLED display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  monitor_check(manual_dig);
  display.setCursor(0,40);
  display.println("Press 5 to back");
  display.setCursor(0,48);
  display.println("Press 4 to exit");
  display.display();
  delay(DELAYTIME);
  //monitor_check(manual_dig);
}

//perform exiting screen
void exiting(){
  int moving = 18;
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Exiting");

  for(int i = 0; i < xpos; i+=moving)
    display.fillRect(i, 30, 16, 16, SSD1306_WHITE);
  display.display();

  xpos = (xpos + moving) % (SCREEN_WIDTH - 16 + moving);}

//perform loading screen 
void loadingScreen(int & xpos){
  int moving = 18;
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Loading");

  for(int i = 0; i < xpos; i+=moving)
    display.fillRect(i, 30, 16, 16, SSD1306_WHITE);
  display.display();

  xpos = (xpos + moving) % (SCREEN_WIDTH - 16 + moving);
}

int checkSerial(){
  while(1)
  {
    if(Serial.available()) 
    {
      String inp = Serial.readString();
      return atoi(inp.c_str());
    }
  }
}

int readBlock(int block_num, byte read_arr[]) 
{
  int sector_num = block_num/4*4;
  int trailer_block = sector_num + 3;

  //authentication 
  byte status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailer_block, &key, &(rfid.uid));

  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(rfid.GetStatusCodeName(status));
         return 3;
  }

//read block
byte buffer_size = 18;
status = rfid.MIFARE_Read(block_num, read_arr, &buffer_size);
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(rfid.GetStatusCodeName(status));
          return 4;
  }
  //Serial.println("The block was read !");
}


int numeric_decode(byte de_arr[])
{
  int result = 0;
  int temp = 0;

  for(int i = 0; i < 16; i++)
  {
    temp = de_arr[i] * pow_up(16,i) ;
    result = result + temp;
   }
   return result;
}



int pow_up(int i , int j)
{
  int t = 1;
  for(int a = 0; a < j; a++)
  {
    t = t*i;
   
   }
   return t;
}



String string_decode(byte de_arr[])
{
  
  String result;
  String temp;
  
  for(int i = 0; i < 16; i++)
  {
    temp = char(de_arr[i]);
    result = result + temp;
    
   }
   return result;
  
}

int userInfo()
{
  byte read_back_array[18];
  Serial.println(F("User Name: "));
  readBlock(6,read_back_array);
  Serial.println(string_decode(read_back_array));


  Serial.print(F("The Money inside the electronic pocket is : $"));
  readBlock(18,read_back_array);
  Serial.print(numeric_decode(read_back_array));
  e_money = numeric_decode(read_back_array);
  return e_money;
}
