#include <Arduino.h>
#include <Servo.h>

// HC-SR04 Ultrasonic Sensor
#define SOS 0.0343 // cm/µs
#define TRIG 8
#define ECHO 9

// 9G Micro Servo
#define SERVO 5
#define MAX_ANGLE 130
#define MIN_ANGLE 50
#define STEP 1
#define SERVO_DELAY 38

// LEDs
#define RED 4
#define GREEN 3

// globals
Servo sm;
int angle = 90;
enum direction {SERVO_RIGHT, SERVO_LEFT};

// prototypes
int ping(void);
void scan(enum direction dir);

void setup()
{
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  sm.attach(SERVO);
  sm.write(angle); 
}

void loop()
{
  scan(SERVO_RIGHT);
  scan(SERVO_LEFT);
}

int ping(void)
{
  float time, dist;

  digitalWrite(TRIG, LOW);
  delay(2);
  digitalWrite(TRIG, HIGH);
  delay(10 );
  digitalWrite(TRIG, LOW);

  time = pulseIn(ECHO, HIGH);
  dist = (time * SOS) / 2;

  return dist > 20 ? 0 : 1;
}

void scan(enum direction dir)
{
  switch (dir) {
    case SERVO_LEFT:
      while (angle <= MAX_ANGLE) {
        sm.write(angle);
        digitalWrite(GREEN, HIGH);
        
        while (ping()) {
          digitalWrite(GREEN, LOW);
          digitalWrite(RED, HIGH);
          delay(2000);
        }
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);

        angle += STEP;
        delay(SERVO_DELAY);
      }
      break;
    
    case SERVO_RIGHT:
      while (angle >= MIN_ANGLE) {
        sm.write(angle);
        digitalWrite(GREEN, HIGH);
        
        while (ping()) {
          digitalWrite(GREEN, LOW);
          digitalWrite(RED, HIGH);
          delay(2000);
        }
        digitalWrite(RED, LOW);
        digitalWrite(GREEN, HIGH);
          
        angle -= STEP;
        delay(SERVO_DELAY);
      }
      break;
      
    default:
      break;
  }
}