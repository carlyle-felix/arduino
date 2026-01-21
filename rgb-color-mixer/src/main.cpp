/*
  adjust rgb led colours in real-time with a rotary encoder and display 
  colour intensities on a 16x2 i2c lcd. 

  press encoder button to cycle through colours
  press pushbutton to reset led
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// push button
#define BUTTON 13

// rgb ky-016
#define RED 3
#define GREEN 5
#define BLUE 6

// rotary encoder ky-040
#define SW 8
#define DT 9
#define CLK 2

LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t clk_prev, sel;
struct rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} rgb_led;
enum colour {Red, Green, Blue};

void lcd_print(void);
void lcd_print_selection(void);
void set_colour(void);
void rotary_encoder(void);

void setup(void)
{
  pinMode(BUTTON, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), rotary_encoder, CHANGE); 
  clk_prev = LOW;
  lcd.init();
  lcd.backlight();
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
}

void loop(void)
{
  // reset rgb struct
  if (digitalRead(BUTTON)) {
    rgb_led.red = 0;
    rgb_led.green = 0;
    rgb_led.blue = 0;
  }

  // select colour
  if (!digitalRead(SW)) {
    delay(150);
    sel++;
  }
  if (sel > Blue) {
    delay(150);
    sel = Red;
  }

  lcd_print_selection();
  lcd_print();
  set_colour();
}

void rotary_encoder(void)
{
  uint8_t clk;
  clk = digitalRead(CLK);
  if (clk != clk_prev) {
    if (digitalRead(DT) != clk) {
      switch (sel) {
        case Red:
          rgb_led.red++;
          break;
        case Green:
          rgb_led.green++;
          break;
        case Blue:
          rgb_led.blue++;
          break;
        default:
          break;
      }
    } else {
      switch (sel) {
        case Red:
          rgb_led.red--;
          break;
        case Green:
          rgb_led.green--;
          break;
        case Blue:
          rgb_led.blue--;
          break;
        default:
          break;
      }
    }
  }
  clk_prev = clk;
}

void set_colour(void)
{
  analogWrite(RED, rgb_led.red);
  analogWrite(GREEN, rgb_led.green);
  analogWrite(BLUE, rgb_led.blue);
}

void lcd_print(void)
{
  uint8_t colour, value;
  uint8_t location_color[] = {1, 7, 13};
  uint8_t location_value[] = {0, 6, 12};
  char color[][3] = {"R", "G", "B"};

  for (colour = 0; colour < 3; colour++) {
    lcd.setCursor(location_color[colour], 0);
    lcd.print(color[colour]);

    switch (colour) {
      case Red:
        value = rgb_led.red;
        break;
      case Green:
        value = rgb_led.green;
        break;
      case Blue:
        value = rgb_led.blue;
        break;
      default:
        break;
    }

    lcd.setCursor(location_value[colour], 1);
    if (value < 10) {
    lcd.print("00");
    } else if (value < 100) {
      lcd.print("0");
    }
    lcd.print(value);
  }
}

void lcd_print_selection()
{
  uint8_t r, c;
  uint8_t location[][2] = {{0, 2}, {6, 8}, {12, 14}};
  uint8_t block[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  
  lcd.createChar(0, block);
  for (r = 0; r < 3; r++) {
    for (c = 0; c < 2; c++) {
      lcd.setCursor(location[r][c], 0);
      if (r == sel) {
        lcd.write(0);
      } else {
        lcd.print(" ");
      }
    }
  }
}