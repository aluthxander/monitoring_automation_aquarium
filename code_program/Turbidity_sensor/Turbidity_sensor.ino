//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 2, 16);
int sensorPin = 34;

void setup()
{
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  //  lcd.begin();
  //  pinMode(2, OUTPUT);
  //  pinMode(3, OUTPUT);
  //  pinMode(4, OUTPUT);
}
void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.print("ADC Sensor: ");
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 2100, 100, 0);
  if (turbidity < 0) {
    turbidity = 0;
  }
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  Serial.println(" NTU");
  //  delay(100);
  //  lcd.setCursor(0, 0);
  //  lcd.print("turbidity:");
  //  lcd.print("   ");
  //  lcd.setCursor(10, 0);
  //  lcd.print(turbidity);
  delay(1000);
  //  if (turbidity < 20) {
  //    digitalWrite(2, HIGH);
  //    digitalWrite(3, LOW);
  //    digitalWrite(4, LOW);
  //    lcd.setCursor(0, 1);
  //    lcd.print(" its CLEAR ");
  //  }
  //  if ((turbidity > 20) && (turbidity < 50)) {
  //    digitalWrite(2, LOW);
  //    digitalWrite(3, HIGH);
  //    digitalWrite(4, LOW);
  //    lcd.setCursor(0, 1);
  //    lcd.print(" its CLOUDY ");
  //  }
  //  if (turbidity > 50) {
  //    digitalWrite(2, LOW);
  //    digitalWrite(3, LOW);
  //    digitalWrite(4, HIGH);
  //    lcd.setCursor(0, 1);
  //    lcd.print(" its DIRTY ");
  //  }
}
