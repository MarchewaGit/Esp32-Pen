#include <Wire.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define MPU9250_ADDR 0x68

long last_sample_millis = 0;
bool capture = false;
char a;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("=== MAGICZNA ROZDZKA - GESTURE CAPTURE ===");

  // Inicjalizacja I2C
  Wire.end();
  Wire.setPins(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  Wire.begin();

  // Sprawdź czujnik
  Wire.beginTransmission(MPU9250_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("CZUJNIK NIE ZNALEZIONY!");
    Serial.println("Sprawdź połączenia!");
    while(1) delay(100);
  }

  Serial.println("CZUJNIK ZNALEZIONY!");

  // Wybudzenie czujnika
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);

  Serial.println("=== GOTOWY ===");
  Serial.println("Wpisz 'o' aby zaczac nagrywanie");
  Serial.println("Wpisz 'p' aby zatrzymac");
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    a = Serial.read();
    if (a == 'o') {
      Serial.println("-,-,-");
      capture = true;
    } else if (a == 'p') {
      capture = false;
      Serial.println("\n\n\n\n");
    }
  }
  
  if (capture) {
    if ((millis() - last_sample_millis) >= 40) {
      last_sample_millis = millis();
      
      Wire.beginTransmission(MPU9250_ADDR);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU9250_ADDR, 6, true);

      if (Wire.available() >= 6) {
        int16_t ax = Wire.read() << 8 | Wire.read();
        int16_t ay = Wire.read() << 8 | Wire.read();
        int16_t az = Wire.read() << 8 | Wire.read();

        float gx = ax / 4096.0;
        float gy = ay / 4096.0;
        float gz = az / 4096.0;

        Serial.print(gx, 3);
        Serial.print(",");
        Serial.print(gy, 3);
        Serial.print(",");
        Serial.println(gz, 3);
      }
    }
  }
}
