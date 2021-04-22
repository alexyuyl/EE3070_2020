int back_exit_manual() {
    Serial.println(F("\n\nPlease Enter 3 to back into the manual!"));
    Serial.println(F("Please Enter 4 for exit!"));
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Enter 3 to back");
    dispay.setCursor(0,10);
    display.println("Enter 4 to exit");
    display.display();
    //delay(DELAYTIME);
    while (1)
    {
      int monitor_receive = checkSerial();
      switch (monitor_receive)
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
