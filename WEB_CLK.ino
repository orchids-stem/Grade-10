#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD I2C setup

#define SDA_PIN 2
#define SCL_PIN 4

// Wi-Fi credentials
#define WIFI_SSID  "your_ssid"     
#define WIFI_PASS  "your_password"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "1.asia.pool.ntp.org", 19800, 60000); // NTP for India (GMT +5:30)

// Time and Date placeholders
char Time[] = "TIME:00:00:00";
char Date[] = "DATE:00/00/2000";

void setup() {
  Serial.begin(115200);
  
  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C with the specified SDA, SCL pins
  lcd.begin();
  lcd.backlight();

  lcd.print("Connecting...");
  connectToWiFi();  // Wi-Fi connection

  timeClient.begin();
  lcd.clear();
}

void loop() {
  timeClient.update();
  updateTime();
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
}

void updateTime() {
  unsigned long unixTime = timeClient.getEpochTime();  // Get time from NTP

  sprintf(Time, "TIME:%02d:%02d:%02d", hour(unixTime), minute(unixTime), second(unixTime));
  sprintf(Date, "DATE:%02d/%02d/%d", day(unixTime), month(unixTime), year(unixTime));

  lcd.setCursor(0, 0);
  lcd.print(Time);

  lcd.setCursor(0, 1);
  lcd.print(Date);

  Serial.println(Time);
  Serial.println(Date);
}
