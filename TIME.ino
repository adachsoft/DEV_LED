void setupTime()
{
  timeClient = new NTPClient(ntpUDP, config.ntpServer, utcOffsetInSeconds);
  timeClient->begin();
}

String getDateTime()
{
  unsigned long epochTime = timeClient->getEpochTime();
  String formattedTime = timeClient->getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  return String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + "T" + formattedTime;
}
