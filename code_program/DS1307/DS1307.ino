#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
DateTime now;

String tanggal, bulan, tahun, jam, menit, detik;
int itahun;

void setup() {
  Serial.begin(115200);
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set waktu langsung dari waktu PC
  bacaRTC();
}

void loop() {
  bacaRTC();
  Serial.println("\n" + konversi_jam(jam) + ":" + konversi_jam(menit) + ":" + konversi_jam(detik) + "\n");
  delay(1000);
}

void bacaRTC()
{
  DateTime now = rtc.now(); // Ambil data waktu dari DS1307
  tanggal = String(now.day(), DEC);
  bulan = String(now.month(), DEC);
  itahun = now.year() - 2000;
  tahun = String(itahun);
  jam = String(now.hour(), DEC);
  menit = String(now.minute(), DEC);
  detik = String(now.second(), DEC);
}

String konversi_jam(String angka) // Fungsi untuk supaya jika angka satuan ditambah 0 di depannya, Misalkan jam 1 maka jadi 01 pada LCD
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
