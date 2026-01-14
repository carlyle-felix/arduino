#include <Arduino.h>

#define WINDOW 500

const uint8_t sensor = 4;   /* ky-037 */
const uint8_t relay = 8;    /* ky-019 */

int8_t clap_count(void);

void setup()
{
  pinMode(sensor, INPUT);
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino clap-on light.");
}

void loop() 
{
  static uint8_t state = LOW;

  if (clap_count()) {
    state = !state;
  }

  if (state) {
    Serial.println("Turn light on.");
    digitalWrite(relay, HIGH);
  } else {
    Serial.println("Turn light off.");
    digitalWrite(relay, LOW);
  }
  delay(100);
}

int8_t clap_count(void)
{
  uint32_t start;

  for (;;) {
    Serial.println("waiting for clap");
    while (!digitalRead(sensor));
    Serial.println("first clap");
    delay(50);
    
    start = millis();
    while ((millis() - start) < WINDOW) {
      if (digitalRead(sensor)) {
        Serial.println("second clap");
        return 1;
      }
    }
  }

  return 0;
}