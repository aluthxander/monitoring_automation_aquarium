#define ph_pin 35
//float po = 0;
//float PH_step;
int nilai_analog_PH;
float nilai_PH;
//double TeganganPh;

void setup() {
  pinMode(ph_pin, INPUT);
  Serial.begin(115200);

}

void loop() {
  nilai_analog_PH = analogRead(ph_pin);
  Serial.print("Nilai ADC Ph: ");
  Serial.println(nilai_analog_PH);
  nilai_PH = (nilai_analog_PH-5223.3)/-285.68;
  Serial.print("Nilai PH: ");
  Serial.println(nilai_PH);
//  TeganganPh = 5.0 / 1024.0 *nilai_analog_PH;
//  Serial.print("Tegangan Ph: ");
//  Serial.println(TeganganPh, 3);

//  PH_step = (PH4-PH7)/3;
//  po = 7.00 * ((PH7-TeganganPh)/PH_step);
//  Serial.print("Nilai PH cairan: ");
//  Serial.println(po, 2);
  delay(1000);

}
