void setup()
{
  Serial.begin(115200);
}

void loop()
{
  int key = checkSerial(); 
  Serial.println(key);
}

int checkSerial()
{
  while(1)
  {
    if(Serial.available()) 
    {
      String inp = Serial.readString();
      return atoi(inp.c_str());
    }
  }
}
