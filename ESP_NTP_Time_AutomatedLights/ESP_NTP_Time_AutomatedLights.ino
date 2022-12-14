#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>

#define light 2
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "MADT";
const char *password = "luckydumpy";
WiFiUDP ntpUDP;

String date_formatted;
String month_formatted;
String clean_date;

// NTPClient timeClient(UDP& udp, const char* poolServerName, int timeOffset (5hr 30 mins -> 19800 sec), unsigned long updateInterval);
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 300000);

String intro[] = {"H", "E", "L", "L", "O", " ", "W", "O", "R", "L", "D"};
String arr_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {
  pinMode(light, OUTPUT);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i <= 10; i++) {
      lcd.setCursor(i + 2, 0);
      lcd.print(intro[i]);
      delay(250);
      Serial.print(".");
    }
  }

  Serial.println("Connected....");
  timeClient.begin();
  lcd.clear();
}

void loop() {
  timeClient.update();

  // Get the Unix Epoch Time
  time_t epochTime = timeClient.getEpochTime();

  // Serial.println(timeClient.getFormattedTime());
  lcd.setCursor(2, 0);

  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int sec = timeClient.getSeconds();

  if (hh < 10) {
    lcd.print(0);
  }
  lcd.print(hh);
  lcd.print(":");

  if (mm < 10) {
    lcd.print(0);
  }
  lcd.print(mm);
  lcd.print(":");

  if (sec < 10) {
    lcd.print(0);
  }
  lcd.print(sec);

  int day = timeClient.getDay();
  lcd.print(" ");
  lcd.print(arr_days[day]);

  // Get date from Epoch Time
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  // store the date, month and year
  int date = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;

  // Format the date
  if (date < 10) {
    date_formatted = (0 + String(date));
  }
  else {
    date_formatted = String(date);
  }

  // Format the month
  if (month < 10) {
    month_formatted = (0 + String(month));
  }
  else {
    month_formatted = String(month);
  }

  clean_date = (date_formatted + "/" + month_formatted + "/" + String(year));
  // uncomment for debug only
  // Serial.println(clean_date);

  lcd.setCursor(3, 1);
  lcd.print(clean_date);

  // condition to turn on/off the light (automated)
  if (hh == 14) {
    digitalWrite(light, LOW);
  }
  // lights will turn off after 1hr from the given time
  else {
    digitalWrite(light, HIGH);
  }
}
