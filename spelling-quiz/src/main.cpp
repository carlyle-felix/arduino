/***************************** Spelling Quiz ***********************************
 *                                                                             *
 * Intentionally easy quiz to help kid sound out words and type them depending *
 * on the number associated with the word. Correct answers triggers a window   *
 * period in which a button can be pressed to display colours on an RGB        *
 *                                                                             *
 ******************************************************************************/

#include <Arduino.h>

// Randomize order (comment out for ordered)
#define RANDOM

// RGB colour scheme (RAINBOW or UNICORN)
#define RAINBOW

// RGB colours macro
#ifdef UNICORN
  #define PINK 255, 20, 147
  #define BLUE 173, 216, 230
  #define PURPLE 128, 0, 128

  enum colours {Pink, Blue, Purple};
#endif

#ifdef RAINBOW
  #define RED 255, 0, 0
  #define ORANGE 255, 108, 0
  #define YELLOW  235, 165, 0
  #define GREEN 0, 255, 0
  #define BLUE 0, 0, 255
  #define PURPLE 128, 0, 128

  enum colours {Red, Orange, Yellow, Green, Blue, Purple};
#endif

// Max words
#define MAX 10

// Store words in array of pointers
const char *wordList[] = {"pup",
                          "cat",
                          "dog",
                          "ant",
                          "wet",
                          "jet",
                          "pan",
                          "hop",
                          "bat",
                          "set"};

// Globals
const uint8_t answerBuzzer = 2;
const uint8_t answerRed = 3;
const uint8_t answerGreen = 4;
const uint8_t red = 8;
const uint8_t green = 9;
const uint8_t blue = 10;
const uint8_t button = 13;

// Prototypes
char *selectWord(uint8_t wordNum);
uint8_t evaluate(char *answer, char *word);
void output(uint8_t result);
void outputReset(void);
void set_colour(const uint8_t r, const uint8_t g, const uint8_t b);
void enableRGB(void);
void printOrdered(void);

void setup()
{
  pinMode(answerBuzzer, OUTPUT);
  pinMode(answerRed, OUTPUT);
  pinMode(answerGreen, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT);

#ifdef RANDOM
  randomSeed(millis());
#endif

  Serial.begin(9600);
  Serial.setTimeout(600000);
}

void loop()
{
  String temp;
  char answer[32];
  uint8_t result = 1;
  static uint8_t wordNum = 0;

#ifdef RANDOM
  uint8_t prevWordNum = wordNum;

  // Avoid repeat words
  while (wordNum == prevWordNum) {
    wordNum = random(0, 10);
  }
#endif

  // Print word list
  printOrdered();

  while (result) {
    // Print message
    Serial.print("Enter word ");
    Serial.print(wordNum + 1);
    Serial.print(": ");

    // Collect and store input
    while (!Serial.available());
    temp = Serial.readStringUntil('\r');
    Serial.println();
    Serial.flush();
    temp.toCharArray(answer, sizeof(answer));

    // Evaluate answer
    result = evaluate(answer, selectWord(wordNum));

    // Output result buzzer and LED
    output(result);  

    // Print message if incorrect
    if (result) {
      Serial.println("Incorrect, try again...");
      Serial.println();
    }
  }

  if (!result) {
    // Correct
    enableRGB();

    // Increment wordNum
    wordNum++;
    if (wordNum > 9) {
      wordNum = 0;
    }
  }
}

char *selectWord(uint8_t wordNum)
{
  if (wordNum < MAX) {
    return (char *) wordList[wordNum];
  }

  Serial.println("error: wordNum beyond MAX");
  return NULL;
}

uint8_t evaluate(char *answer, char *word)
{
  if (!strcmp(answer, word)) {
    return 0;
  }

  return 1;
}

void output(uint8_t result)
{
  if (!result) {
    digitalWrite(answerGreen, HIGH);
    tone(answerBuzzer, 10);
    delay(100);
    noTone(answerBuzzer);
    delay(100);
    tone(answerBuzzer, 13);
    delay(200);
  } else {
    digitalWrite(answerRed, HIGH);
    tone(answerBuzzer, 8);
    delay(100);
    noTone(answerBuzzer);
    delay(150);
    tone(answerBuzzer, 5);
    delay(500);
  }
  noTone(answerBuzzer);
  outputReset();
}

void outputReset(void)
{
  digitalWrite(answerGreen, LOW);
  digitalWrite(answerRed, LOW);
}

void set_colour(const uint8_t r, const uint8_t g, const uint8_t b)
{
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}

void enableRGB(void)
{
  uint8_t colour = -1;
  uint32_t start, duration;

  // Duration
  start = millis();
  duration = 10000;

  // Button logic
  while (millis() < (start + duration)) {
    if (digitalRead(button)) {
      colour++;
      delay(30);
    } else {
      set_colour(0, 0, 0);
      continue;
    }

#ifdef UNICORN
    if (colour > 2) {
      colour = 0;
    }

    switch (colour) {
      case Pink:    set_colour(PINK);
                    break;
      case Blue:    set_colour(BLUE);
                    break;
      case Purple:  set_colour(PURPLE);
                    break;
      default:      break;
    }
    delay(500);
#endif

#ifdef RAINBOW
    if (colour > 5) {
      colour = 0;
    }

    switch (colour) {
      case Red:     set_colour(RED);
                    break;
      case Orange:  set_colour(ORANGE);
                    break;
      case Yellow:  set_colour(YELLOW);
                    break;
      case Green:   set_colour(GREEN);
                    break;
      case Blue:    set_colour(BLUE);
                    break;
      case Purple:  set_colour(PURPLE);
                    break;
      default:      break;
    }
    delay(500);
#endif
  }
  set_colour(0, 0, 0);
}

void printOrdered(void)
{
  uint8_t wordNum;

  Serial.println();
  Serial.println("Word list:");
  Serial.println();
  for (wordNum = 0; wordNum < MAX; wordNum++) {
    Serial.print(wordNum + 1);
    Serial.print(".\t");
    Serial.println((char *) wordList[wordNum]);
  }
  Serial.println();
}