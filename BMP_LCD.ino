#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration (address 0x27, 16x2 display)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SDA_PIN 2
#define SCL_PIN 4

#define RED_LED 22
#define GREEN_LED 23

float temp, pressure_kPa, altitude;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  // Set up LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);  // Initially turn off both LEDs
  digitalWrite(GREEN_LED, HIGH);

  // Set up I2C communication with the same SDA and SCL pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize LCD
  lcd.begin();
  lcd.backlight();

  // Display initial message on the LCD
  lcd.print("LCD I2C Module");
  delay(2000);  // Display message for 2 seconds
  lcd.clear();  // Clear the LCD screen

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("BMP180 sensor not found, check wiring!");
    lcd.clear();
    lcd.print("Sensor Error!");
    while (1);  // Stop further execution if sensor is not found
  }
  delay(10);  // Small delay after initializing sensor
}

void loop() {
  // Read sensor data
  readSensorData();

  // Control LED behavior based on conditions
  if (temp >= 29 && pressure_kPa < 92) {
    digitalWrite(RED_LED, LOW);   // Turn ON Green LED, OFF Red LED
    digitalWrite(GREEN_LED, HIGH);
  } else if (temp < 29 && pressure_kPa < 92) {
    digitalWrite(RED_LED, HIGH);  // Turn ON Red LED, OFF Green LED
    digitalWrite(GREEN_LED, LOW);
  } else if (pressure_kPa > 92) {
    digitalWrite(RED_LED, LOW);   // Turn OFF both LEDs if pressure is high
    digitalWrite(GREEN_LED, LOW);
  }

  delay(2000);  // Delay to slow down the loop and give time for display update
}

void readSensorData() {
  // Read temperature
  temp = bmp.readTemperature();
  Serial.printf("Temperature = %.2f°C\n", temp);

  // Read pressure in kPa
  pressure_kPa = bmp.readPressure() / 1000;
  Serial.printf("Pressure = %.2f kPa\n", pressure_kPa);

  // Read altitude
  altitude = bmp.readAltitude();
  Serial.printf("Altitude = %.2f meters\n", altitude);

  // Display temperature on the first line of the LCD
  lcd.clear();  // Clear the previous LCD data
  lcd.setCursor(0, 0);  // Set the cursor to the first line
  lcd.printf("Temp: %.2f C", temp);  // Print temperature on LCD

  // Display pressure on the second line of the LCD
  lcd.setCursor(0, 1);  // Set the cursor to the second line
  lcd.printf("Pres: %.2f kPa", pressure_kPa);  // Print pressure on LCD

  // Optional: Display altitude in the serial monitor
  Serial.printf("Altitude = %.2f meters\n", altitude);
}
