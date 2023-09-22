#define BLYNK_TEMPLATE_ID "_ _ _"
#define BLYNK_TEMPLATE_NAME "_ _ _"
#define BLYNK_AUTH_TOKEN "_ _ _"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <NewPing.h>
#include <OneWire.h> //Call the required OneWire library as the Dallas Temperature dependency library
#include <DallasTemperature.h> // Calls the Dallas Temperature library
#include <Wire.h>
#include "RTClib.h"// calls the DS1307 RTC library
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 23  // DS18B20 sensor pin. 
#define ph_pin 35 // PH sensor pin
#define Trig 26 // ultrasonic trig pin
#define Echo 25 // ultrasonic echo pin
#define turbidityPin 34 // turbidity sensor pin
#define servoPin 19 // Servo pin
#define relay1 18 // relay1 pin
#define relay2 5 // relay2 pin

// Maximum distance that can be measured by an ultrasonic sensor (in centimeters)
#define MAX_DISTANCE 200
#define tinggiAkuarium 30 // Enter the height of the aquarium in cm
int distance = 0, tinggiAir; // variable to accommodate the ultrasonic sensor distance and water height values

OneWire oneWire(ONE_WIRE_BUS); // Create a oneWire variable based on the PIN that has been defined
DallasTemperature sensor(&oneWire); // Create a variable to store measurement results
RTC_DS1307 rtc; // create an RTC variable
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address and LCD size
DateTime now;
NewPing sonar(Trig, Echo, MAX_DISTANCE);
Servo myservo;

float suhuDS18B20; // DS18B20 temperature variable declaration with float data type
String tanggal, bulan, tahun, jam, menit, detik;
int itahun;
int nilai_analog_PH; // accommodates the results of the PH sensor ADC readings
float nilai_PH; // accommodates the actual PH value results
int turbidityADC = 0;
int turbidityValue = 0;
char auth[] = BLYNK_AUTH_TOKEN;
unsigned long previousMillis = 0;
unsigned long interval = 2000;  // Time interval for LCD displays alternately (ms)
int state = 0;  // Variable to move the lcd display
bool koneksiWifi = false;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "_ _ _";
char pass[] = "_ _ _";

BlynkTimer timer;
WidgetLED makanIkan(V0);
WidgetLED pompa1(V5);
WidgetLED pompa2(V6);

// to find out whether the ESP32 is connected to blynk IoT or not
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void wifiOnline() {
  Blynk.virtualWrite(V1, tinggiAir);
  Blynk.virtualWrite(V2, suhuDS18B20);
  Blynk.virtualWrite(V3, nilai_PH);
  Blynk.virtualWrite(V4, turbidityValue);
  if (jam.equals("16") && menit.equals("35") && detik.equals("0")) {
    Serial.println("Waktunya memberi makan ikan");
    makanIkan.on();
    for (int pos = 0; pos < 180; pos += 1) // loop function which will be made into PWM with increments of 1
    {
      myservo.write(pos); // procedure for writing PWM data to the servo motor
      delay(15); //delay 15 ms
    }
    delay(1000);
    for (int pos = 180; pos >= 1; pos -= 1) // the loop function will be made into PWM with a decrease of 1
    {
      myservo.write(pos);
      delay(15);
    }
    makanIkan.off();
  }

  if (turbidityValue > 25) {
    pompa1.on();
    while (tinggiAir > (tinggiAkuarium * 0.5)) {
      Serial.println("Sedang Menguras Air");
      digitalWrite(relay1, LOW);
      sensorUltrasonic();
      Serial.print("tinggi Air :");
      Serial.println(tinggiAir);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menguras Air");
      lcd.setCursor(0, 1);
      lcd.print("TinggiAir: ");
      lcd.print(tinggiAir);
      lcd.print(" cm");
      Blynk.virtualWrite(V1, tinggiAir);
      delay(500);
    }
    pompa1.off();
    digitalWrite(relay1, HIGH);
    pompa2.on();
    while (tinggiAir < tinggiAkuarium * 0.9) {
      Serial.println("Sedang Mengisi Air");
      digitalWrite(relay2, LOW);
      sensorUltrasonic();
      Serial.print("tinggi Air :");
      Serial.println(tinggiAir);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mengisi Air");
      lcd.setCursor(0, 1);
      lcd.print("Tinggi Air: ");
      lcd.print(tinggiAir);
      lcd.print(" cm");
      Blynk.virtualWrite(V1, tinggiAir);
      delay(500);
    }
    pompa2.off();
    digitalWrite(relay2, HIGH);
  }
}

void wifiOffline() {
  if (jam.equals("10") && menit.equals("35") && detik.equals("0")) {
    Serial.println("Waktunya memberi makan ikan");
    for (int pos = 0; pos < 180; pos += 1) // loop function which will be made into PWM with increments of 1
    {
      myservo.write(pos); // procedure for writing PWM data to the servo motor
      delay(15); // delay time 15 ms
    }
    delay(1000);
    for (int pos = 180; pos >= 1; pos -= 1) // the loop function will be made into PWM with a decrease of 1
    {
      myservo.write(pos);
      delay(15);
    }
  }

  if (turbidityValue > 25) {
    while (tinggiAir > (tinggiAkuarium * 0.5)) {
      Serial.println("Sedang Menguras Air");
      digitalWrite(relay1, LOW);
      sensorUltrasonic();
      Serial.print("tinggi Air :");
      Serial.println(tinggiAir);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menguras Air");
      lcd.setCursor(0, 1);
      lcd.print("TinggiAir: ");
      lcd.print(tinggiAir);
      lcd.print(" cm");
      delay(500);
    }
    digitalWrite(relay1, HIGH);
    while (tinggiAir < tinggiAkuarium * 0.9) {
      Serial.println("Sedang Mengisi Air");
      digitalWrite(relay2, LOW);
      sensorUltrasonic();
      Serial.print("tinggi Air :");
      Serial.println(tinggiAir);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mengisi Air");
      lcd.setCursor(0, 1);
      lcd.print("Tinggi Air: ");
      lcd.print(tinggiAir);
      lcd.print(" cm");
      delay(500);
    }
    digitalWrite(relay2, HIGH);
  }
}

// function to send data to blynk
void myTimerEvent()
{
  //  check wifi connection
  checkWiFiConnection();
  //  when a wifi connection exists
  if (koneksiWifi) {
    wifiOnline();
  } else {//when there is no wifi connection
    wifiOffline();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ph_pin, INPUT);
  pinMode(turbidityPin, INPUT);
  myservo.attach(servoPin);
  sensor.begin();      //Initiate One-Wire DS18B20 sensor
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  lcd.init();  // Initialize LCD
  lcd.backlight();   // Activates the backlight on the LCD
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, myTimerEvent);

  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set waktu langsung dari waktu PC
  bacaRTC();
}

void loop() {
  bacaRTC();
  sensorPh();
  sensorUltrasonic();
  sensorSuhu();
  sensorTurbidity();
  tampilkanSemuaSensor();
  unsigned long currentMillis = millis();  // Gets the current time

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Use state to determine the display to be displayed
    switch (state) {
      case 0:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(konversi_jam(jam) + ":" + konversi_jam(menit) + ":" + konversi_jam(detik));
        // check wifi connection
        if (koneksiWifi) {
          lcd.setCursor(0, 1);
          lcd.print("Online");
        } else {// when there is no wifi connection
          lcd.setCursor(0, 1);
          lcd.print("Offline");
        }
        state = 1;  // Move to the next state
        break;
      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TinggiAir: ");
        lcd.print(tinggiAir);
        lcd.print(" cm");
        lcd.setCursor(0, 1);
        lcd.print("Suhu: ");
        lcd.print(suhuDS18B20);
        lcd.print(" C");
        state = 2;  // Move to the next state
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("pH: ");
        lcd.print(nilai_PH);
        lcd.setCursor(0, 1);
        lcd.print("Kekeruhan: ");
        lcd.print(turbidityValue);
        lcd.print(" NTU");
        state = 0;// move to the next state
    }
  }


  //  delay(1000);
  Blynk.run();
  timer.run();

}

void checkWiFiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    koneksiWifi = true;
  } else {
    koneksiWifi = false;
  }
}

void sensorPh() {
  nilai_analog_PH = analogRead(ph_pin);
  //  Serial.print("Nilai ADC Ph: ");
  //  Serial.println(nilai_analog_PH);
  nilai_PH = (nilai_analog_PH - 5223.3) / -285.68;
  //  Serial.print("Nilai PH: ");
  //  Serial.println(nilai_PH);
}

void sensorUltrasonic() {
  distance = sonar.ping_cm();
  tinggiAir = tinggiAkuarium - distance;
  if (tinggiAir < 0) {
    tinggiAir = 0;
  }
}

void sensorSuhu() {
  sensor.setResolution(11);
  sensor.requestTemperatures();  // Temperature conversion command
  suhuDS18B20 = sensor.getTempCByIndex(0);  // Reads temperature data from sensor #0 and converts it to Celsius values

}

void sensorTurbidity() {
  turbidityADC = analogRead(turbidityPin);
  //  Serial.print("ADC Sensor: ");
  //  Serial.println(turbidityADC);
  turbidityValue = map(turbidityADC, 0, 2100, 100, 0);
  if (turbidityValue < 0) {
    turbidityValue = 0;
  }

}

void tampilkanSemuaSensor() {
  //  displays time
  Serial.println("\n" + konversi_jam(jam) + ":" + konversi_jam(menit) + ":" + konversi_jam(detik));
  //  displays the water level
  Serial.print("Tinggi Air : ");
  Serial.print(tinggiAir);
  Serial.println(" cm");
  // displays the water temperature value
  Serial.print("Suhu : ");
  Serial.print(suhuDS18B20, 3);  // 3 digit precision
  Serial.println(" C");

  //  displays the PH value of water
  Serial.print("Nilai PH: ");
  Serial.println(nilai_PH);
  //  displays the turbidity value
  Serial.print("Turbidity: ");
  Serial.print(turbidityValue);
  Serial.println(" NTU");
  Serial.println();
}

void bacaRTC()
{
  DateTime now = rtc.now(); // Retrieve time data from DS1307
  tanggal = String(now.day(), DEC);
  bulan = String(now.month(), DEC);
  itahun = now.year() - 2000;
  tahun = String(itahun);
  jam = String(now.hour(), DEC);
  menit = String(now.minute(), DEC);
  detik = String(now.second(), DEC);
}

String konversi_jam(String angka) // The function is so that if you add 0 to the units digit in front of it, for example at 1 o'clock it becomes 01 on the LCD
{
  if (angka.length() == 1)
  {
    angka = "0" + angka;
  }
  else
  {
    angka = angka;
  }
  return angka;
}
