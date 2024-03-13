class MedicineActiveReminder {

    int hourForMedicine;
    int minutesForMedicine;
    int hourToShutDownBox;
    int ledIndex;
    boolean tookMedicineInRequiredTime;
    boolean songReminderPlayed;
    boolean messageReminderSent;
    boolean isReminderForMorning;

  public:
    MedicineActiveReminder(int index, int hour, int minutes, int hourToShutDown, boolean isForMorning){
      ledIndex = index;
      hourForMedicine = hour;
      minutesForMedicine = minutes;
      tookMedicineInRequiredTime = false;
      songReminderPlayed = false;
      messageReminderSent = false;
      hourToShutDownBox = hourToShutDown;
      isReminderForMorning = isForMorning;
    }

    void setHourToGetMedicine(int newHour){
      hourForMedicine = newHour;
    }

    void setMinutesToGetMedicine(int newMinutes){
      minutesForMedicine = newMinutes;
    }
    
    void setLedColor(uint32_t color){
      strip.setPixelColor(ledIndex, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
    }

    void playMusicReminder(int currentHour, int currentMinutes){
      if ((songReminderPlayed == false) and (tookMedicineInRequiredTime == false) 
          and (currentHour == hourForMedicine) and (currentMinutes == minutesForMedicine)){
        if(isReminderForMorning == true){
          playMorningMusicReminder();
        }
        else {
          playNightMusicReminder();
        }
        songReminderPlayed = true;
      }
      else {
        if (currentHour >= hourToShutDownBox) {
          songReminderPlayed = false;
        }
      }
    }
    
    void setLedColorByTime(int currentHour, int currentMinutes){
      if (currentHour >= hourToShutDownBox){
         setLedColor(strip.Color(0,   0,   0));
      }
      else if (tookMedicineInRequiredTime == false){
        if (hourForMedicine == currentHour){
          setLedColor(strip.Color(255,   255,   0)); // yellow
        }
        else if (hourForMedicine <  currentHour) {
          setLedColor(strip.Color(255,   0,   0)); // red
        }  
      }
   }

   void tookeMedicineValidator(int currentHour, int minimumHourToTakeMedicine, int maximunHourToTakeMedicine){
    int touckensorValue = touchRead(T0);
    // Serial.println("touch senseor value: ");
    // Serial.println(touckensorValue);
    if ((touckensorValue < 30) and (currentHour < maximunHourToTakeMedicine)
         and (currentHour >= minimumHourToTakeMedicine) and (tookMedicineInRequiredTime == false)){
      Serial.println("user touched button to take pills, touch sensor value: ");
      Serial.println(touckensorValue);
      tookMedicineInRequiredTime = true;
      setLedColor(strip.Color(0,   255,   0)); // green
    }
   }

   void sendReminderToPhone(int currentHour, int currentMinutes){
     if ((messageReminderSent == false) and (currentHour == hourForMedicine) and (currentMinutes == minutesForMedicine)
         and (tookMedicineInRequiredTime == false)){
      Serial.println("Sending mesage to google calander");
      Blynk.virtualWrite(V0, 1);
      Blynk.virtualWrite(V0, 0);
      messageReminderSent = true;
     }
     if (currentHour >= hourToShutDownBox){
      messageReminderSent = false;
     }
   } 
};
