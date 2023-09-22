#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C dan ukuran LCD

unsigned long previousMillis = 0;
unsigned long interval = 2000;  // Interval waktu dalam milidetik (ms)

int state = 0;  // Variabel untuk melacak status tampilan

void setup() {
  lcd.init();  // Menginisialisasi LCD
  lcd.backlight();   // Mengaktifkan backlight pada LCD
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");
}

void loop() {
  unsigned long currentMillis = millis();  // Mendapatkan waktu saat ini

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Menggunakan state untuk menentukan tampilan yang akan ditampilkan
    switch (state) {
      case 0:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hasil pH");
        lcd.setCursor(0, 1);
        lcd.print("129");
        state = 1;  // Pindah ke state berikutnya
        break;
      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hasil TDS");
        lcd.setCursor(0, 1);
        lcd.print("1213234");
        state = 0;  // Pindah ke state berikutnya
        break;
    }
  }
}
