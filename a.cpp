
  Serial.println("Please Scan your EMP Card ");
  checkCardScan(1);
  empInfo();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  Serial.println("Attendance is took!");

  // update thingspeak server
  int x = ThingSpeak.writeField(AttCH, 1, 1, AttWK);
  if (x != 200){
    Serial.println("not yet taken attendance!");
    while (x != 200){
      x = ThingSpeak.writeField(AttCH, 1, 1. AttWK);
      delay(5000)
    }
    Serial.println("Attendance updated to cloud"):
  else Serial.println("Attendance updated to cloud");
  }
