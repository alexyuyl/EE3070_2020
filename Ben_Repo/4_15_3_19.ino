#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ThingSpeak.h>
#include <WiFiEsp.h>

#define SS_PIN 53
#define RST_PIN 5


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DELAYTIME 500 //need further testing

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MFRC522 rfid(SS_PIN, RST_PIN); // Create object Instance
MFRC522::MIFARE_Key key; //create variable “key” with MIFARE_Key structure

int tmpc;

char ssid[] = "ww";
char pass[] = "wififortest";
int status = WL_IDLE_STATUS;
WiFiEspClient client;

int statuscode = 0;
int number = 0;

unsigned long int UserCH = 1311029;
char UserWK[] = "CG3TUFU1WV7LN5VV";
char UserRK[] = "IQBMLYWYCX7BFX0N";

unsigned long int G1CH = 1357809;
char G1WK[] = "Z3D6H5Z7LAYGWAL1";
char G1RK[] = "0UGP4B38Q61MQKRY";

unsigned long G2CH = 1357810;
char G2WK[] = "XF9C7PC40OGB5IQ4";
char G2RK[] = "Y7SY4DU99B4GO2IA";

unsigned long int AttCH = 1357815;
char AttWK = "2GMTKO6DKD70ZZQH";
char AttRK = "A2TF6FM0AB6BL9CN";

void setup() { 
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init the MFRC522 chip
  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
    }


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

  ThingSpeak.begin(client);
}

void loop() {

  mainMenu(); //plain display function for main menu
  int manualDig = checkSerial(); //user input 
  //plsScanCard(); //display function for pls scan card
  Serial.println("------------Please scacn your user card First------------");
  checkCardScan(1);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
  switch(manualDig)
  {
    case 1:
      //check whether the card is a user card
      //return cardCheck = true if it is a user card, vice versa
      shopMode(); //customer page, shopping mode enabled
      break;
      
    case 2:
      //check whether the card is an employee card
      //return cardCheck = true if it is a employee card, vice versa
      empMode(); //employee page, employee mode enabled 
      break;
      
    case 3:
        writeEMP_info();
      break;
    case 4:
        writeProduct_info();
      break;
    default:
        Serial.println("Invalid Input!! Please Enter agian!");
        return;
 }
  
}



//-------------------------------------------------------------------------Functions is Below-------------------------------------------------------------------------




//display function for main menu
void mainMenu(){ 
 /* display.clearDisplay(); //init OLED display manual
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
  delay(DELAYTIME);*/

  Serial.println(F("Please Enter 1 for Entering The Shopping Mode"));
  Serial.println(F("Please Enter 2 for Entering The Employee Page"));
  Serial.println(F("Please Enter 3 for Writing User Info"));
  Serial.println(F("Please Enter 4 for Writing Product Info"));
  
  
}

//display pls scan card
void plsScanCard(){ 
  
  display.clearDisplay(); //init OLED display manual
  display.setTextSize(1);
  display.setCursor(0,32);
  display.println("Pls Scan Your Card");
  display.display();
  //display.startscrolldiagright(0x00, 0x0F);
  delay(DELAYTIME);
}





//  ********************************************************************************************************************************************

void checkCardScan(int mode)
{ 
  
  // mode 1 check whether is user card 
  // mode 2 check whether is product tag
  bool success = false;

  while(!success){
    
   success = true;
  if ( ! rfid.PICC_IsNewCardPresent())
  {
    success = false;
    
    } // Check whether there is PICC card
      
  if ( ! rfid.PICC_ReadCardSerial())//Read the card, if true, then continue
  {
    success = false;
  }
            
  MFRC522::PICC_Type piccType;
  piccType = rfid.PICC_GetType(rfid.uid.sak);
    
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
      {
          success = false;
      }
      
   byte cardInfo[18];
  // check whether is a user card
  if(mode == 1 && success)
  {
    readBlock(1, cardInfo);
    if(cardInfo[0] != 1){
      // print "Not a user card"
      Serial.println("Not user card");
      success = false;
    }
  }

  else if(mode == 2 && success)
  {
    readBlock(1, cardInfo);
    if(cardInfo[0] != 0 || cardInfo[15] != 0){
      // print "Not a product tag"
      Serial.println("Not product tag");
      success = false;
    }
  }

  // stop cryto
   
   if(success)
   {
      return;
    }
   rfid.PICC_HaltA();
   rfid.PCD_StopCrypto1();


 }
}


void scanProduct(int mode, int& userMoney)
{

  //mode 1 for viewing product info, will ignore userMoney
  //mode 2 for purchasing 

  while(1)
  {
    
    if(checkExit())
      return;
    else
      checkCardScan(2);  

      if(mode == 1)
      {
        productInfo();
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
      }
      else if(mode == 2)
      {
        purchase(userMoney);
        rfid.PICC_HaltA();
        rfid.PCD_StopCrypto1();
        
        if(userMoney < 0)
          return;
      }

      /*
      //print product info
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Next product to scan");
      display.display();
      delay(DELAYTIME);  
      //print "Next product to scan" 
      */

    /*
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
    } */


     Serial.println("\nNext product to scan");

  }
}

//  ********************************************************************************************************************************************



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
    /*display.setCursor(0,0);
    display.println("Customer Manual:");
    display.setCursor(0,8);
    display.println("1:Show Product Info");
    display.setCursor(0,16);
    display.println("2:Take Consumption");
    display.setCursor(0,24);
    display.println("3:Leave Manual");
    display.display();
    delay(DELAYTIME);//may need testing
    */
   Serial.println("\n\n\n---------------------------------Wellcome To The Shopping Manual---------------------------------");
   Serial.println();
   Serial.println(F("Please Enter 1 for showing your Product info!"));
   Serial.println(F("Please Enter 2 for taking Consumption!"));
   Serial.println(F("Please Enter 3 for leaving the manual!\n\n"));
   
   int manualDig = checkSerial();

    switch(manualDig)
    {
      case 1: //show product info
              Serial.println(F("Please Scanned The Product Tags To Show The Info "));
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
              int temp = 0;
              exiting( temp);
              return;
      default:
            Serial.println("Invalid Input!! Please Enter agian!");
            return;
    }
  }
}

void shProduct(){
  
  /*display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Please Scanned The Product Tags To Show The Info ");
  display.setCursor(0,16);
  display.println("Enter 5 to finish scanning");
  display.display();
  //delay(DELAYTIME); */
  int userMoney  = 0;
  scanProduct(1,userMoney);
}




bool checkExit()
{

  delay(3000);
  if(Serial.available()) 
    {
      String inp = Serial.readString();
      int manualDig =  atoi(inp.c_str());
      if(manualDig == 5)
      {
        
        Serial.println(F("Now To Exit!"));
        return true;
      }
      
      else{
        /*
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("Invalid Input");
        display.setCursor(0,8);
        display.println("Enter 5 to finish");
        display.setCursor(0,16);
        display.println("Next product to scan");
        display.display();
        delay(DELAYTIME);*/
        Serial.println("Invalid input");
        Serial.println("Enter 5 to finish");
        Serial.println("Or Next product to scan");
      }
        
    }
   return false;
  
}

void takeConsumption(){

  //scan user card first

  int userMoney;
  Serial.println(F("Please Scanned Your User Card For Taking Consumption "));
   checkCardScan(1);
   userMoney = userInfo();

   // stop check card
   rfid.PICC_HaltA();
   rfid.PCD_StopCrypto1();

 /* 
  else
  {
    // print invalid card
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Invalid Card");
    display.display();
    delay(DELAYTIME);
    return;
  } */

 /* display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Please Scanned The Product Tags To Show The Info ");
  display.display();
  //display.startscrolldiagright(0x00, 0x0F);
  delay(DELAYTIME); */

    Serial.println("User Card Confirmed! ");
    Serial.println("Please Scanned The Product Tags for purchasing");

    scanProduct(2,userMoney);

    Serial.println("Scan your user card For Final Confirmation ");
    checkCardScan(1);
    byte writeVal[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    store_dec(writeVal,userMoney,0);
    writeBlock(18,writeVal);
    ThingSpeak.writeField(UserCH, 1, userMoney, UserWK);

    //stop scan user card
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    
  /*
  else
  {
    //print invalid card
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Invalid Card");
    display.display();
    delay(DELAYTIME);
    return;
  } */

  
}

void empMode(){
    //display.clearDisplay();//init OLED for text output
    //display.setTextSize(1);
    //display.setTextColor(WHITE);
    bool terminate = false;
  while(!terminate)
  {
    /*
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
    delay(DELAYTIME);*/


    Serial.println("\n\n\n---------------------------------Wellcome to the Employee Manual---------------------------------");
    Serial.println();
    Serial.println(F("Please Enter 1 for showing your personal info!"));
    Serial.println(F("Please Enter 2 for taking attendence!"));
    Serial.println(F("Please Enter 3 for viewing all the product info!"));
    Serial.println(F("Please Enter 4 for leaving the manual!\n\n"));
    int manualDig = checkSerial();   

    switch(manualDig)
    {
      case 1: //show emp personal info
              Serial.println("Please Scan your Employee Card !");
              checkCardScan(1);
              empInfo();
              rfid.PICC_HaltA();
              rfid.PCD_StopCrypto1();
              if(back_exit_manual() == 4)
              {
                return;
              }
                
              break;

      case 2: //take attendence
              takeAttence();
              if(back_exit_manual() == 4)
              {
                  return;
                }
              break;

      case 3: //viewing all product info
              break;
      case 4: //leave manual
              terminate = true;
              break;
      default:
              Serial.println("Invalid Input!! Please Enter agian!");
              return;
    }
    
  }
}

int back_exit_manual(){
  
   Serial.println(F("\n\nPlease Enter 3 to back into the manual!"));
   Serial.println(F("Please Enter 4 for exit!"));
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
  if (tmpc == 1){
    float discount = ThingSpeak.readFloatField(G1CH, 3, G1RK);
    statuscode = ThingSpeak.getLastReadStatus();
    if (statuscode == 200){
    price = price * (1-discount);
    }
    else{
      while (statuscode != 200){
        discount = ThingSpeak.readFloatField(G1CH, 3, G1RK);
        delay(5000);
      }
      price = price * (1-discount);
    }     
  }
  else if (tmpc == 2){
    float discount = ThingSpeak.readFloatField(G2CH, 3, G2RK);
    statuscode = ThingSpeak.getLastReadStatus();
    if (statuscode == 200){
    price = price * (1-discount);
    }
    else{
      while (statuscode != 200){
        discount = ThingSpeak.readFloatField(G2CH, 3, G2RK);
        delay(5000);
      }
      price = price * (1-discount);
    }     
  }

  
   userMoney = userMoney - price;

   
  if(userMoney < 0)
  {
    //print not enough money
    /*
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
    } */
    Serial.println("!!!! Not Enough Money in Account !!!!");
    return;
  }

  readBlock(18,read_back_array);
  int consumTimes = numeric_decode(read_back_array);
  consumTimes = consumTimes + 1;

  byte default_val[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  store_dec(default_val,consumTimes,0);
  writeBlock(18,default_val);
  readBlock(18,read_back_array);
  
  
}

void productInfo()
{

  byte read_back_array[18];
  //display.clearDisplay(); //init OLED display
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.setCursor(0,0);
  Serial.print(F("\nThe Product Name is : "));
  readBlock(6,read_back_array);
  String tmp = string_decode(read_back_array);
  if (tmp == "Ice Cream")
    tmpc = 1;
  else if (tmp == "pudding")
    tmpc = 2;
  
  Serial.println(tmp);
  //Serial.println();
  //display.println("Name:");
  //display.setCursor(0,8);
  //display.println(tmp);
    
  Serial.print(F("\nThe Product Type is : "));
  readBlock(10,read_back_array);
  tmp = string_decode(read_back_array);
  Serial.println(tmp);
  //Serial.println();
  //display.setCursor(0,16);
  //display.println(tmp);

  Serial.print(F("\nThe Price Of The Product : "));
  readBlock(14,read_back_array);
  tmp = numeric_decode(read_back_array);
  if (tmpc == 1){
    int x = ThingSpeak.writeField(G1CH, 2, tmp, G1WK);
    if(x == 200){
      Serial.println("Price of Ice Cream update successful");
    }
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    while (x != 200){
      x = ThingSpeak.writeField(G1CH, 2, tmp, G1WK);
      delay(5000);
    }
    Serial.println("Price of Ice Cream update successful");
    }
  }
  else if (tmpc == 2){
     int x = ThingSpeak.writeField(G2CH, 2, tmp, G2WK);
    if(x == 200){
      Serial.println("Price of pudding update successful");
    }
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    while (x != 200){
      x = ThingSpeak.writeField(G2CH, 2, tmp, G2WK);
      delay(5000);
    }
    Serial.println("Price of pudding update successful");
    }
  }
  Serial.println(tmp);
  //display.setCursor(0,24);
  //display.println("Price($) :");
  //display.setCursor(0,32);
  //display.println(tmp);
  //display.display();

  Serial.print(F("\nThe Product has been consumed for : "));
  //display.setCursor(0,40);
  //display.println("Consumed for");
  readBlock(18,read_back_array);
  int tmp2 = numeric_decode(read_back_array);
  int ori_stock;
  int stocks;
  if (tmpc == 1){
   ori_stock = ThingSpeak.readFloatField(G1CH, 1, G1RK);
   statuscode = ThingSpeak.getLastReadStatus();
   if (statuscode != 200){
    Serial.println("Failed to read Ice Cream Stocks");
    while (statuscode != 200){
      ori_stock = ThingSpeak.readFloatField(G1CH, 1, G1RK);
    }
   }
  }
  else if (tmpc == 2){
    ori_stock = ThingSpeak.readFloatField(G2CH, 1, G2RK);
   statuscode = ThingSpeak.getLastReadStatus();
   if (statuscode != 200){
    Serial.println("Failed to read Ice Cream Stocks");
    while (statuscode != 200){
      ori_stock = ThingSpeak.readFloatField(G2CH, 1, G2RK);
    }
   }
  }
  stocks = ori_stock - tmp2;
  if (tmpc == 1){
    int x = ThingSpeak.writeField(G1CH, 1, stocks, G1WK);
    if(x == 200){
      Serial.println("Stocks of Ice Cream update successful");
    }
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    while (x != 200){
      x = ThingSpeak.writeField(G1CH, 2, stocks, G1WK);
      delay(5000);
    }
    Serial.println("Stocks of Ice Cream update successful");
    }
  }
  else if (tmpc == 2){
     int x = ThingSpeak.writeField(G2CH, 1, stocks, G2WK);
    if(x == 200){
      Serial.println("Stocks of pudding update successful");
    }
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    while (x != 200){
      x = ThingSpeak.writeField(G2CH, 1, stocks, G2WK);
      delay(5000);
    }
    Serial.println("Stocks of pudding update successful");
    }
  }
   
   
   
  Serial.print(numeric_decode(read_back_array));
  //display.setCursor(0,48);
  //display.println(numeric_decode(read_back_array));
  Serial.print(F(" times."));
  Serial.println();
}



void empInfo(){ //Print emp personal info
  
  byte read_back_array[18];

/*
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
  int manualDig = checkSerial();
  display.setCursor(0,40);
  display.println("Press 5 to back");
  display.setCursor(0,48);
  display.println("Press 4 to exit");
  display.display();
  delay(DELAYTIME);
  manualDig = checkSerial();*/

  Serial.println();
  Serial.print(F("Employee Name: "));
  readBlock(6,read_back_array);
  Serial.print(string_decode(read_back_array));
  Serial.println();

  Serial.print(F("The Job position is : "));
  readBlock(10,read_back_array);
  Serial.print(string_decode(read_back_array));
  Serial.println();

  Serial.print(F("The salary is : $"));
  readBlock(14,read_back_array);
  Serial.print(numeric_decode(read_back_array));
  Serial.println();
  
}

void takeAttence(){ //take attence function for emp
  //internet

  display.clearDisplay(); //init OLED display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  int manualDig = checkSerial();
  display.setCursor(0,40);
  display.println("Press 5 to back");
  display.setCursor(0,48);
  display.println("Press 4 to exit");
  display.display();
  delay(DELAYTIME);
  //monitor_check(manual_dig);
}

//perform exiting screen
void exiting(int & xpos){
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
  String tmp = string_decode(read_back_array);
  Serial.println(tmp);

  /*
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("User name:");
  display.setCursor(0,8);
  display.println(tmp);
  display.setCursor(0,16);
  display.println("Balance:"); */
  
  Serial.print(F("The Money inside the electronic pocket is : $"));
  readBlock(18,read_back_array);
  int tmp1 = numeric_decode(read_back_array);
  Serial.print(tmp1);
  Serial.println();
  /*
  display.setCursor(0,24);
  display.println(tmp1);
  display.display();
  delay(DELAYTIME); */
  
  int e_money = numeric_decode(read_back_array);
  return e_money;
}


void store_dec(byte result_arr[], int val , int index)
{
  if(val >= 16 )
    store_dec(result_arr, val/16 , index + 1);

  result_arr[index] = val%16;
}


int writeBlock(int block_num, byte write_arr[]) 
{
  int sector_num = block_num/4*4;
  int trailer_block = sector_num + 3;
  if (block_num == trailer_block){
    Serial.println(F("The block to write is a trailer block! "));
    return 3;
    }


  //authentication 
  byte status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailer_block, &key, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(rfid.GetStatusCodeName(status));
         return 4;
  }
  
  //writing  block 
  status = rfid.MIFARE_Write(block_num, write_arr, 16);
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(rfid.GetStatusCodeName(status));
           return 5;
  }
}




int writeProduct_info()
{   
  Serial.println("-------------Scan your product tag to change info-------------");
  bool success = false;
  while(!success)
  {
    success = true;
    
    if ( ! rfid.PICC_IsNewCardPresent())
        success = false;

    if ( ! rfid.PICC_ReadCardSerial())
         success = false;

     MFRC522::PICC_Type piccType;
     piccType = rfid.PICC_GetType(rfid.uid.sak);
    
     if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
          success = false;
    
      if(success)
      {
        
        Serial.print(F("\n\n-----------------------------------------------Write Default Product Info Here-----------------------------------------------"));
        // the block for checking the info wrote only
        byte re_block[18];  
        
        //  byte 0 of identify employee info or product info --> 0 or 1
        //  byte 1 for is represents the security level    ----> 1,2 or 3
        // product tag [0] == 0 , [15] == 0
        byte default_block1[16] = {0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        writeBlock(1,default_block1);
        readBlock(1,re_block);
        //printHex(re_block,block_size-2);
       // Serial.println("\n------------------------------------------------------------------------------");
    
    
    
        // 0 for numeric info
        // 1 for string stored info
        byte S_info[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
        
        //block 4 --> sector 1
        S_info[0] = 1;
        S_info[1] = 1;
        writeBlock(4,S_info);
        //block 5 is (Name:)
        byte default_block3[16] = {78,97,109,101,32,58,0,0,0,0,0,0,0,0,0,0};
        writeBlock(5,default_block3);
    
        // ********  block 6 is the name of the product 
        // Ice Cream 73 99 101 32 67 114 101 97 109 
        // hamburger 104 97 109 98 117 114 103 101 114 
        // pudding 112 117 100 100 105 110 103 10 
        byte default_block4[16] = {73 ,99 ,101 ,32 ,67 ,114 ,101 ,97 ,109, 0,0,0,0,0,0,0};
        writeBlock(6,default_block4);
        readBlock(6,re_block);
       // printHex(re_block,block_size-2);
        //string_decode(re_block);
       // Serial.println("\n------------------------------------------------------------------------------");
    
    
        //block 8 --> sector 2 
        S_info[0] = 1;
        S_info[1] = 1;
        writeBlock(8,S_info);
        //block 9  is  (Job Position:)
        //74 111 98 32 80 111 115 105 116 105 111 110 58 
        byte default_block5[16] = {74,111,98,32,80,111,115,105,116,105,111,110,58,0,0,0};
        writeBlock(9,default_block5);
    
    
    
        //******** block 10  is product type
        // dessert 100 101 115 115 101 114 116 
        // food 70 111 111 100 
        byte default_block6[16] = {100 ,101 ,115 ,115 ,101 ,114 ,116,0,0,0,0,0,0,0,0,0};
        writeBlock(10,default_block6);
        //readBlock(10,re_block);
        //printHex(re_block,block_size-2);
       // string_decode(re_block);
       // Serial.println("\n------------------------------------------------------------------------------");
    
    
    
        //block 12 ---> sector 3
        S_info[0] = 1;
        S_info[1] = 0;
        writeBlock(12,S_info);
        //block 13 is (Salary:)
        // 83 97 108 97 114 121 58 
        byte default_block7[16] = {83,97,108,97,114,121,58,0,0,0,0,0,0,0,0,0};
        writeBlock(13,default_block7);
    
        // ********* block 14 Price , default $30 1E
        byte default_block8[16] = {13,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        writeBlock(14,default_block8);
        readBlock(14,re_block);
        tmpc = 1; //Uncomment the line that you want to reset
        //tmpc = 2;
        if (tmpc == 1){
          int x = ThingSpeak.writeField(G1CH, 2, 30, G1WK);
          if(x == 200){
            Serial.println("Price of Ice Cream update successful");
          }
        else {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
          while (x != 200){
          x = ThingSpeak.writeField(G1CH, 2, 30, G1WK);
          delay(5000);
          }
          Serial.println("Price of Ice Cream update successful");
          }
        }
       else if (tmpc == 2){
        int x = ThingSpeak.writeField(G2CH, 2, 30, G2WK);
        if(x == 200){
          Serial.println("Price of pudding update successful");
        }
        else {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
          while (x != 200){
           x = ThingSpeak.writeField(G2CH, 2, 30, G2WK);
           delay(5000);
          }
         Serial.println("Price of pudding update successful");
        }
       }
        //printHex(re_block,block_size-2);
        //numeric_decode(re_block);
        //Serial.println("\n------------------------------------------------------------------------------");
        
        
        //block 18 number of times being consummed
        byte default_block9[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        writeBlock(18,default_block9);
        readBlock(14,re_block);
        tmpc = 1; //Uncomment the line that you want to reset
        //tmpc = 2;
        /*if (tmpc == 1){
          int x = ThingSpeak.writeField(G1CH, 1, 100, G1WK);
          if(x == 200){
            Serial.println("Price of Ice Cream update successful");
          }
        else {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
          while (x != 200){
          int x = ThingSpeak.writeField(G1CH, 1, 100, G1WK);
          delay(5000);
          }
          Serial.println("Price of Ice Cream update successful");
          }
        }
       else if (tmpc == 2){
        int x = ThingSpeak.writeField(G2CH, 1, 100, G2WK);
        if(x == 200){
          Serial.println("Price of pudding update successful");
        }
        else {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
          while (x != 200){
           x = ThingSpeak.writeField(G2CH, 100, tmp, G2WK);
           delay(5000);
          }
         Serial.println("Stocks of pudding update successful");
        }
       }*/
       // printHex(re_block,block_size-2);
       // numeric_decode(re_block);
       // Serial.println("\n------------------------------------------------------------------------------");
    
       // store_dec(re_block,333,0);
        
        
        // last block is taking scanning number of times.
        Serial.print(F("\n\nFinished !"));
        
       }
   rfid.PICC_HaltA();
   rfid.PCD_StopCrypto1();
   if(success)
     return;
  }
}


int writeEMP_info()
{
    Serial.println("-------------Scan your User card to change info-------------");
    bool success = false;
    while(!success)
    {
      
      success = true;
      
      if ( ! rfid.PICC_IsNewCardPresent())
      {
        success = false;
      }
  
      if ( ! rfid.PICC_ReadCardSerial())
      {
        success = false;
      }
           
  
       MFRC522::PICC_Type piccType;
       piccType = rfid.PICC_GetType(rfid.uid.sak);
      
       if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
              piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
              piccType != MFRC522::PICC_TYPE_MIFARE_4K)
              {
                success = false;
                }
      if(success)
      {
          
        Serial.print(F("\n\n-----------------------------------------------Write Default Employee Info Here-----------------------------------------------"));
        // the block for checking the info wrote only
        byte re_block[18];  
    
        //  byte 0 of identify employee info or product info --> 0 or 1 && last byte == 1
        //  byte 1 for is represents the security level    ----> 1,2 or 3
        //  last byte = 1 ---> emp
        byte default_block1[16] = {1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
        writeBlock(1,default_block1);
        readBlock(1,re_block);
       // printHex(re_block,block_size-2);
       // Serial.println("\n------------------------------------------------------------------------------");
    
    
    
        
        // 0 for numeric info
        // 1 for string stored info
        byte S_info[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
        
        //block 4 --> sector 1
        S_info[0] = 1;
        S_info[1] = 1;
        writeBlock(4,S_info);
        //block 5 is (Name:)
        byte default_block3[16] = {78,97,109,101,32,58,0,0,0,0,0,0,0,0,0,0};
        writeBlock(5,default_block3);
    
        //***********  block 6 is the name of the employee 
        // Chan Tai Man
        // 67 104 97 110 32 84 97 105 32 77 97 110 
        byte default_block4[16] = {67,104,97,110,32,84,97,105,32,77,97,110,0,0,0,0};
        writeBlock(6,default_block4);
        readBlock(6,re_block);
        //printHex(re_block,block_size-2);
        //string_decode(re_block);
        //Serial.println("\n------------------------------------------------------------------------------");
    
    
        //block 8 --> sector 2 
        S_info[0] = 1;
        S_info[1] = 1;
        writeBlock(8,S_info);
        //block 9  is  (Job Position:)
        //74 111 98 32 80 111 115 105 116 105 111 110 58  
        byte default_block5[16] = {74,111,98,32,80,111,115,105,116,105,111,110,58,0,0,0};
        writeBlock(9,default_block5);
        //**************block 10  is job position
        // Manager   77 97 110 97 103 101 114 
        byte default_block6[16] = {77,97,110,97,103,101,114,0,0,0,0,0,0,0,0,0};
        writeBlock(10,default_block6);
        readBlock(10,re_block);
       // printHex(re_block,block_size-2);
        //string_decode(re_block);
       // Serial.println("\n------------------------------------------------------------------------------");
    
    
    
        //block 12 ---> sector 3
        S_info[0] = 1;
        S_info[1] = 0;
        writeBlock(12,S_info);
        //block 13 is (Salary:)
        // 83 97 108 97 114 121 58 
        byte default_block7[16] = {83,97,108,97,114,121,58,0,0,0,0,0,0,0,0,0};
        writeBlock(13,default_block7);
    
        // ***************block 14 salary , default $19999 4E1F
        byte default_block8[16] = {15,1,14,4,0,0,0,0,0,0,0,0,0,0,0,0};
        writeBlock(14,default_block8);
        readBlock(14,re_block);
       // printHex(re_block,block_size-2);
       // numeric_decode(re_block);
       // Serial.println("\n------------------------------------------------------------------------------");
    
       // ************block 18 will be used record electronic packet money of the user
       // better for demo, employee can also be a user
       // $537 --> 219(hex)
        byte default_block9[16] = {9,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0};
        writeBlock(18,default_block9);
        readBlock(18,default_block9);
        Serial.println("stage 1");
        int x = ThingSpeak.writeField(UserCH, 1, 537, UserWK);
        Serial.println("Stage 2");
        if (x == 200){
          Serial.println("User Money reset successful");
        }
        else{
          Serial.println("User Money reset failed");
          while( x != 200){
            x = ThingSpeak.writeField(UserCH, 1, 537, UserWK);
            delay(5000);
          }
          Serial.println("User Money reset successful");
        }
   
        // remaining block for attendence taking
        // last block is taking scanning number of times.
        Serial.print(F("\n\nFinished!"));
          
      }
      
     rfid.PICC_HaltA();
     rfid.PCD_StopCrypto1();
     if(success)
       return;
      }
    
}
