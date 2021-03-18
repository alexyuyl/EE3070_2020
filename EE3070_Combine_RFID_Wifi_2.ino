#include <SPI.h>
#include <MFRC522.h>
#include <WiFiEsp.h>
#include <ThingSpeak.h>
#define SS_PIN 53
#define RST_PIN 5

//wifi setting
char ssid[] = "EE3070_P1800_1";
char pass[] = "EE3070P1800";
int status = WL_IDLE_STATUS;
WiFiEspClient client;

unsigned long CHID = 1324991;
int statuscode = 0;
int number = 0;
char WriteKey[] = "8KQ0UIBFYBVEJ1KQ";

// RGB
#define R1 40
#define G1 41
#define B1 42
#define block 1


byte default_val[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte read_back_array[18];
byte block_size = 18;
int manual_dig;  //  manual digit used in serial read
int manual_dig_cb; // clear buffer digit
int temp;  
bool started = false;
bool shopping = false;
bool emp_mode = false;
int consumption_val = 0;


MFRC522 rfid(SS_PIN, RST_PIN); // Create object Instance
MFRC522::MIFARE_Key key; //create variable “key” with MIFARE_Key structure

void setup() {

 
// led setting 
 //set both baud rate
  Serial.begin(115200);
  
  Serial1.begin(115200);

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
    printWifiData();
    ThingSpeak.begin(client);//thingspeak initialization
  }
pinMode(R1,OUTPUT);
pinMode(G1,OUTPUT);
pinMode(B1,OUTPUT);  
Serial.begin(115200);
SPI.begin(); // Init SPI bus
rfid.PCD_Init(); // Init the MFRC522 chip
 for (byte i = 0; i < 6; i++) {
 key.keyByte[i] = 0xFF;
 }
 //Serial.println(F("This code scan the MIFARE Classsic NUID."));
// Serial.print(F("Using the following key:"));
// printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

}

void loop() {



if(!started)
{
    Serial.println(F("\nPlease Enter 1 for Entering The Customer Page "));
    Serial.println(F("\nPlease Enter 2 for Entering The Employee Page"));
    monitor_check(manual_dig);
    monitor_check(manual_dig_cb); // clear buffer
    

     // Reset the loop if no new card present on the sensor/reader. This saves the entire process when
    //idle.

    scan_card_user();
    print_start_manual();
   
}

if(emp_mode)
{
    print_emp_mode_manual();

} 


if(shopping)
{
    print_shopping_manual();
}


}



//-----------------------------------------------------Functions----------------------------------------------------------



//-----------RFID related Functions-------------------------



//----loop start ---- main manul
void print_start_manual()
{
  started = true;
    if(manual_dig == 1)
    {
        //Shopping Mode
        shopping = true;
        emp_mode = false;
        
        readBlock(1,read_back_array);
        if(read_back_array[0] != 1)
        {
            Serial.println(F("\nThe Card Scanned Is Not A User Card !!"));
            shopping = false;
            started = false;
            rfid.PICC_HaltA();
            // Stop encryption on PCD
            rfid.PCD_StopCrypto1();
            return;
       
        }


    }

    if(manual_dig == 2)
    {
        //emp Mode
        shopping = false;
        emp_mode = true;


        readBlock(1,read_back_array);
        if(read_back_array[0] != 1 || read_back_array[15] != 1) 
        {
            Serial.println(F("\nThe Card Scanned Is Not A Employee Card !"));
            emp_mode = false;
            started = false;
            rfid.PICC_HaltA();
            // Stop encryption on PCD
            rfid.PCD_StopCrypto1();
            return;
       
        }


    }

    if(manual_dig == 3)
    {
        Serial.println(F("Write The default EMP Info Into The Card/Tag\n\n"));
        writeEMP_info();
        started = false;
        rfid.PICC_HaltA();
       // Stop encryption on PCD
        rfid.PCD_StopCrypto1();
        return;


    }

    if(manual_dig == 4)
    {
        Serial.println(F("Write The default Product Info Into The Card/Tag\n\n"));
        writeProduct_info();
        started = false;
       rfid.PICC_HaltA();
      // Stop encryption on PCD
       rfid.PCD_StopCrypto1();
       return;
    }
  
  
  
  
  
 }


// Manual-Printing Function ---- Employee Part---------------------------------------

void print_emp_mode_manual()
{

  bool terminate = false;
  while(!terminate)
 {
  
  Serial.println("\n\n\n--------------------------Wellcome to the Employee Manual--------------------------");
  Serial.println();
  Serial.println(F("Please Enter 1 for showing your personal info!"));  // print out employee info like 1. employee name 2. employee job position  3. employee salary
  Serial.println(F("Please Enter 2 for taking attendence!"));  // Not finished Yet
  Serial.println(F("Please Enter 3 for viewing all the product info!")); // Not finished Yet
  Serial.println(F("Please Enter 4 for leaving the manual!\n\n")); // leave the employee manual
  
  monitor_check(manual_dig);

  if(manual_dig == 1)
  {
    print_empInfo();  // all the values like employee name, job postion , salary can be obtained inside <<print_empInfo()>> 
    monitor_check(manual_dig); // flush buffer
    Serial.println(F("\n\n***Please Enter 5 to back into the manual!***"));
    Serial.println(F("***Please Enter 4 for exit!***"));
    monitor_check(manual_dig); // receive manual digit 
  }

   if(manual_dig == 2)
  {
    // need to connect to the internet to get time
    Serial.println(" Opps Still working on it !");
    monitor_check(manual_dig); // flush buffer
    Serial.println(F("\n\n***Please Enter 5 to back into the manual!***"));
    Serial.println(F("***Please Enter 4 for exit!***"));
    monitor_check(manual_dig);  
  }


   if(manual_dig == 3)
  {
    readBlock(1,read_back_array);
    if(read_back_array[1] == 4)
    {
      // need to have the help of database
      Serial.println(" #######Opps Still working on it !#######");
      
    }

    else
    {
      Serial.println("!!!!!!!You are not allowed to view these information!!!!!!!");
      analogWrite(R1,255);
      analogWrite(G1,0);
      analogWrite(B1,0);
      delay(1000);
      analogWrite(R1,0);
    }

    monitor_check(manual_dig); // flush buffer
    
    Serial.println(F("\n\nPlease Enter 5 to back into the manual!"));
    Serial.println(F("Please Enter 4 for exit!"));
    monitor_check(manual_dig);
    
  }

   if(manual_dig == 4)
  {
    terminate = true;
    Serial.println("Now To Exit");
  }

   if(manual_dig == 5)
  {
     monitor_check(manual_dig); // flush buffer
  }

  
 }

 emp_mode = false;
 started = false;
 monitor_check(manual_dig); // buffer clear


 // Halt PICC
 rfid.PICC_HaltA();
 // Stop encryption on PCD
 rfid.PCD_StopCrypto1();

}
// Manual-Printing Function Employee Part----------END Here---------------------------------------




// Manual-Printitng Function ------------- customer part-------------------------------------------

void print_shopping_manual()
{

  
    bool terminate = false;
    while(!terminate)
    {


            Serial.println("\n\n\n--------------------------Wellcome To The Customer Manual--------------------------");
            Serial.println();
            Serial.println(F("Please Enter 1 for showing your Product info!"));  // SHOW Product Info Like  1. Product Name  2. Product Type  3. Product Price
            Serial.println(F("Please Enter 2 for taking Consumption!"));        // The product is Consumed when The product is scanned , After the consumption , the total cost will be provided
            Serial.println(F("Please Enter 3 for leaving the manual!\n\n"));   // Leave Customer Manual and back to the Main Manual
            
            monitor_check(manual_dig);
           
            if(manual_dig == 1)
            {
                Serial.println(F("Please Scanned The Product Tags To Show The Info "));
                rfid.PICC_HaltA();
                rfid.PCD_StopCrypto1();
                while(!scan_card(false));   // The values of product type, product price , product name  can be obtained in Function <<print_productInfo()>>
                //scan_card();
                monitor_check(manual_dig); // flush buffer
                Serial.println(F("\n\n***Please Enter 4 to back into the manual!***"));
                Serial.println(F("***Please Enter 3 for exit!***"));
                monitor_check(manual_dig);
            }

        if(manual_dig == 2)
        {
            Serial.println(F("Please Scanned The Product Tags For Taking Consumption "));
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            consumption_val = 0;
            while(!scan_card(true));      // The values of product type, product price , product name  can be obtained in Function <<print_productInfo()>>
            monitor_check(manual_dig); // flush buffer

            scan_card_user();
            int user_money;
            print_user_info(user_money);
            
            Serial.print(F("\n\n*****The Total Cost For Of The Current Consumption is : $"));
            Serial.print(consumption_val);     // consumption_val is total cost after consumption  
            Serial.println(" *****\n\n");
            Serial.print("The E-Money inside the Pocket : $ ");
            Serial.println(user_money - consumption_val);
             store_dec(default_val,user_money - consumption_val,0);
            writeBlock(18,default_val);
            ThingSpeak.writeField(CHID, 1, user_money, WriteKey);
            consumption_val = 0;
            Serial.println(F("\n\n***Please Enter 4 to back into the manual!***"));
            Serial.println(F("***Please Enter 3 for exit!***"));
            monitor_check(manual_dig);
        }



        if(manual_dig == 3)
        {
            terminate = true;
            Serial.println("Now To Exit");
        }

        if(manual_dig == 4)
        {
            monitor_check(manual_dig); // flush buffer
        }


    }

    shopping = false;
    started = false;
    monitor_check(manual_dig); // flush buffer
     // Halt PICC
   rfid.PICC_HaltA();
   // Stop encryption on PCD
   rfid.PCD_StopCrypto1();


}

// Manual-Printitng Function customer part-End Here---------------------------------------------------



// Function To Write RFID Card 
int writeBlock(int block_num, byte write_arr[]) 
{
  int sector_num = block_num/4*4;
  int trailer_block = sector_num + 3;
  if (block_num == trailer_block){
    Serial.println(F("The block to write is a trailer block! "));
    return 3;
    }

  // Serial.println(F("The block to write is a data block! "));

  
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
  //Serial.println("The block was written !");
}


// Function To Read RFID Card

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





// Function To Print The Info of The Product
void print_productInfo(bool consumption)
{


    
    readBlock(1,read_back_array);
    if(read_back_array[0] == 1 && read_back_array[15] == 1)
    {
      Serial.println(F("\n Invalid card Detected \n\n"));
      return;
    }
    
    
    Serial.println(F("\n\n>>>>>>>>>>>Here will show the product information<<<<<<<<<<<"));
  

    Serial.print(F("\nThe Product Name is : "));
    readBlock(6,read_back_array);
    string_decode(read_back_array);
    
    Serial.print(F("\nThe Product Type is :"));
    readBlock(10,read_back_array);
    string_decode(read_back_array);

    Serial.print(F("\nThe Price Of The Product : "));
    readBlock(14,read_back_array);
    Serial.print(numeric_decode(read_back_array));
    consumption_val = consumption_val + numeric_decode(read_back_array);

    Serial.print(F("\nThe Product has been consumed for : "));
    readBlock(18,read_back_array);
    int temp_re = numeric_decode(read_back_array);
    
    if(consumption)
    {
      store_dec(default_val,temp_re+1,0);
      writeBlock(18,default_val);
      temp_re = temp_re + 1;
      ThingSpeak.writeField(CHID, 2, temp_re, WriteKey);
    }
    
    Serial.print(temp_re);
    Serial.print(F(" times."));
}




// Function To Print Employee Info


void print_empInfo()
{

  Serial.println();
  Serial.print(F("Employee Name: "));
  readBlock(6,read_back_array);
  string_decode(read_back_array);
  Serial.println();

  Serial.print(F("The Job position is : "));
  readBlock(10,read_back_array);
  string_decode(read_back_array);
  Serial.println();

  Serial.print(F("The salary is : $"));
  readBlock(14,read_back_array);
  Serial.print(numeric_decode(read_back_array));
  Serial.println();


  
}


void print_user_info(int& e_money)
{
  Serial.println();
  Serial.print(F("User Name: "));
  readBlock(6,read_back_array);
  string_decode(read_back_array);
  Serial.println();


  Serial.print(F("The Money inside the electronic pocket is : $"));
  readBlock(18,read_back_array);
  Serial.print(numeric_decode(read_back_array));
  Serial.println();
  e_money = numeric_decode(read_back_array);
}


// Function To retrieve Input Value from Serial Monitor
void monitor_check(int & man_dig)
{

  bool enter_dig = false;
  
  while(! enter_dig)
  {

      if(Serial.available() > 0)
    {
      man_dig = Serial.parseInt();
      enter_dig = true;
      
    }
  }
}


// Function To scan The Card

bool scan_card(bool consumption)
{   
    bool terminate = false;
    int a_temp = consumption;
    while(!terminate)
    {

        int serial_digit;
        if(Serial.available() > 0)
        {
            serial_digit = Serial.parseInt();
            if(serial_digit == 5)
            {
                
                Serial.println(F("\n>>>>>Now To Exit!<<<<<"));
                terminate = true;
                return true;
                //monitor_check(manual_dig); // flush buffer
            }
            

        }
        
        if ( ! rfid.PICC_IsNewCardPresent()) // Check whether there is PICC card
        return false;
        // Verify if the NUID has been read
        if ( ! rfid.PICC_ReadCardSerial()) //Read the card, if true, then continue
        return false;

        
        MFRC522::PICC_Type piccType;
        piccType = rfid.PICC_GetType(rfid.uid.sak);


        if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return false;
        }

        
        
        print_productInfo(a_temp);
        
        // Halt PICC
        rfid.PICC_HaltA();
        // Stop encryption on PCD
        rfid.PCD_StopCrypto1();

        Serial.print(F("\n\n~~~~~~~~~~~Next Product To Scanned~~~~~~~~~~~\n"));
        Serial.println(F("***Enter 5 For Ending The Scanning***\n\n"));

    }
   
    return true;
    
}

// card sacn for employee/user card  

void scan_card_user()
{

    Serial.println(F("\n\n----------------------------Please Scanned Your User Card----------------------------\n"));
    bool not_scanned = true;
    while(not_scanned)
    {      
          not_scanned = false;
          
        if ( ! rfid.PICC_IsNewCardPresent()) // Check whether there is PICC card
            not_scanned = true ;
            // Verify if the NUID has been read
        if ( ! rfid.PICC_ReadCardSerial()) //Read the card, if true, then continue
            not_scanned = true ;
    
            
        MFRC522::PICC_Type piccType;
        piccType = rfid.PICC_GetType(rfid.uid.sak);
    
    
       if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
        {
            not_scanned = true;
         }
         
      }
  
  
}




// Change The decimal val into Hex array 
void store_dec(byte result_arr[], int val , int index)
{
  if(val >= 16 )
    store_dec(result_arr, val/16 , index + 1);

  result_arr[index] = val%16;
}


// Functions For decoding 
//--------------------------------------------------
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
    Serial.print(temp);
    result = result + temp;
    
   }
   return result;
  
}

//---------------------------------------------------




/**
* Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
 Serial.print(buffer[i] < 0x10 ? " 0" : " ");
 Serial.print(buffer[i], HEX);
 }
}

/**
* Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
 Serial.print(buffer[i] < 0x10 ? " 0" : " ");
 Serial.print(buffer[i], DEC);
 }
}






//------------Function Related To WiFi Module --------------
void printWifiData() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}





//--------------------------------------------------------------






// RFID Write Default Values To The Card/Tag
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
int writeProduct_info()
{   



    Serial.print(F("\n\n-----------------------------------------------Write Default Product Info Here-----------------------------------------------"));
    // the block for checking the info wrote only
    byte re_block[18];  
    
    //  byte 0 of identify employee info or product info --> 0 or 1
    //  byte 1 for is represents the security level    ----> 1,2 or 3
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

    // block 6 is the name of the product 
    // Ice Cream
    // 73 99 101 45 67 114 101 97 109 
    byte default_block4[16] = {73,99,101,45,67,114,101,97,109,0,0,0,0,0,0,0};
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
    //block 10  is product type
    // dessert 100 101 115 115 101 114 116 
    byte default_block6[16] = {100,101,115,115,101,114,116,0,0,0,0,0,0,0,0,0};
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

    // block 14 Price , default $30 1E
    byte default_block8[16] = {15,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    writeBlock(14,default_block8);
    readBlock(14,re_block);
    //printHex(re_block,block_size-2);
    //numeric_decode(re_block);
    //Serial.println("\n------------------------------------------------------------------------------");
    
    
    //block 18 number of times being consummed
    byte default_block9[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    writeBlock(18,default_block9);
    readBlock(14,re_block);
   // printHex(re_block,block_size-2);
   // numeric_decode(re_block);
   // Serial.println("\n------------------------------------------------------------------------------");

   // store_dec(re_block,333,0);
    
    
    // last block is taking scanning number of times.
    Serial.print(F("\n\nFinished !"));

}



int writeEMP_info()
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

    // block 6 is the name of the employee 
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
    //block 10  is job position
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

    // block 14 salary , default $19999 4E1F
    byte default_block8[16] = {15,1,14,4,0,0,0,0,0,0,0,0,0,0,0,0};
    writeBlock(14,default_block8);
    readBlock(14,re_block);
   // printHex(re_block,block_size-2);
   // numeric_decode(re_block);
   // Serial.println("\n------------------------------------------------------------------------------");

   // block 18 will be used record electronic packet money of the user
   // better for demo, employee can also be a user
   // $533 --> 219(hex)
    byte default_block9[16] = {9,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0};
    writeBlock(18,default_block9);
    readBlock(18,default_block9);

   
    
    // remaining block for attendence taking
    


    // last block is taking scanning number of times.
    Serial.print(F("\n\nFinished!"));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
