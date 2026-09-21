#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD config (address 0x27, 16x2 display)

#define SDA_PIN 2
#define SCL_PIN 4

#define RED_LED 22
#define GREEN_LED 23

float temp, pressure_kPa, altitude;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);

  Wire.begin(SDA_PIN, SCL_PIN);   // Set up I2C communication with the same pins

  lcd.begin();
  lcd.backlight();

  lcd.print(" LCD I2C Module ");
  delay(2000);
  lcd.clear();

  if (!bmp.begin()) {
    Serial.println("BMP180 sensor not found, check wiring!");
    while (1);
  }
  delay(10);
}

void loop() {
  readSensorData();

  if (temp >= 29 && pressure_kPa < 92) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  } else if (temp < 29 && pressure_kPa < 92) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else if (pressure_kPa > 92) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }

  delay(2000);  // Delay to slow down the loop
}

void readSensorData() {
  // Read temperature
  temp = bmp.readTemperature();
  Serial.printf("Temperature = %.2f°C\n", temp);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("Temp: %.2f C", temp);

  // Read pressure in kPa
  pressure_kPa = bmp.readPressure() / 1000;
  Serial.printf("Pressure = %.2f kPa\n", pressure_kPa);

  lcd.setCursor(0, 1);
  lcd.printf("Pressure: %.2f kPa", pressure_kPa);

  // Read altitude
  altitude = bmp.readAltitude();
  Serial.printf("Altitude = %.2f meters\n", altitude);
}
