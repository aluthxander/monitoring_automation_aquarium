#include <NewPing.h>

#define Trig 26
#define Echo 25
// Maksimum jarak yang dapat diukur oleh sensor ultrasonik (dalam centimeter)
#define MAX_DISTANCE 200
int distance = 0;


NewPing sonar(Trig, Echo, MAX_DISTANCE);



void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(50);
  distance = sonar.ping_cm();
  
  Serial.print("Jarak : ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
  

}
