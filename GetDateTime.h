
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 3600;
const int   daylightOffset_sec = 0;

int getCurrentHour(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }
  
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  return atoi(timeHour);
}

int getCurrentMinutes(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }
  
  char timeMinutes[3];
  strftime(timeMinutes,3, "%M", &timeinfo);
  return atoi(timeMinutes);
}
