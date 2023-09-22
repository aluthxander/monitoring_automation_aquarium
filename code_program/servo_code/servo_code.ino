#include <ESP32Servo.h>

Servo myservo;
int servoPin = 19; // Ganti nomor pin sesuai dengan yang Anda gunakan, di sini saya menggunakan GPIO2 (pin D2) pada ESP32

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin);
}

void loop() {
  for (int pos = 0; pos < 180; pos += 1) //fungsi perulangan yang akan dijadikan PWM dengan kenaikan 1
  {
    myservo.write(pos); //prosedur penulisan data PWM ke motor servo
    delay(15); //waktu tunda 15 ms
  }
  for (int pos = 180; pos >= 1; pos -= 1) //fungsi perulangan yang akan dijadikan PWM dengan penurunan 1
  {
    myservo.write(pos);
    delay(15);
  }
}
