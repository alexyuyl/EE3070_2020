String readString = "{\"created_at\":\"2021-04-07T07:03:32Z\",\"entry_id\":20281,\"status\":null} "; //16 and 36
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

void setup(){
  Serial.begin(115200);
  //get string of year, month, day and hour from current utc time
  utcTime = readString.substring(15,35);
  utcYear = utcTime.substring(0,4);
  utcMonth = utcTime.substring(5,7);
  utcDay = utcTime.substring(9,10);
  utcHour = utcTime.substring(12,13);
  temp1 = utcTime.substring(15,16);
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

void loop()
{

}
