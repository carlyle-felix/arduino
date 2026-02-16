/*
  adjust servo angle in real-time with a rotary encoder and display 
  position on a 16x2 i2c lcd. 
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// rotary encoder ky-040
#define DT 3
#define CLK 2

// servo
#define SERVO 5
#define STEP 5
#define MAX_ANGLE 180
#define MIN_ANGLE 0

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t clk_prev, sel;

Servo sm;
int angle = 90;

void rotary_encoder(void);

void setup(void)
{
  pinMode(DT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), rotary_encoder, CHANGE); 
  clk_prev = LOW;

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Position:");

  sm.attach(SERVO);
  sm.write(angle);
}

void loop(void)
{
  int n;

  lcd.setCursor(0, 1);
  lcd.print(angle);
  if (angle < 100) {
    lcd.print(" ");
  } else if (angle < 10) {
    lcd.print("  ");
  }
}

void rotary_encoder(void)
{
  uint8_t clk;
  clk = digitalRead(CLK);
  if (clk != clk_prev) {
    if (digitalRead(DT) != clk && angle > MIN_ANGLE) {
      angle -= STEP;
      sm.write(angle);
    } else if (digitalRead(DT) == clk && angle < MAX_ANGLE) {
      angle += STEP;
      sm.write(angle);
    }
  }
  clk_prev = clk;
}