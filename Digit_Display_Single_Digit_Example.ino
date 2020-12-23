
/*
  Digit Display Single Digit Example
  Justin Claude Kirk 2020
  info@makerinchief.com
  www.makerinchief.com

  Use with Digit Display Kit
  https://makerinchief.store/product/led-digit-display-kit/


  The Digit Display uses 7 segments of WS2812B LEDs to display numbers 0-9 and
  a few other characters.  The characters are displayed by turning off or on a set
  of segments.  There are 7 segments in the digit and are labeled as follows:

              B
          - - - - -
        |           |
        |           |
      A |           | C
        |           |
  IN ->  |     D     |
          - - - - -
        |           | -> OUT
        |           |
      E |           | G
        |           |
        |           |
          - - - - -
              F


      Essentially, the digit is a single strip of LEDs, broken up into 7 segments of 3.  By turning on or off segments,
      we can display a character.  For example, to display the number 1, you would turn on segment C and G while turning off
      the rest.  Using WS2812B you can also control the color of each pixel offering a wide range of color options.

      This example shows how to set the color of a digit and display the numbers 0-9.  See comments for more notes.

      Enjoy!


*/


//This works using the FastLED library to control the WS2812 LEDs.

#include <FastLED.h>

//The analog pin 0 of the Arduino Nano is used just for convience.  Any GPIO pin can be used.
const int LED_PIN = 14; //Analog Pin A0 GPIO number


//Essentially, we are creating a long single strip of leds which get broken up into segments based on the following:

//Number of total digits
const int DIGIT_COUNT = 1;

//Number of leds in each segment
const int SEGMENT_COUNT = 3;

//Number of segments for all digits
//Each digt needs 7 segments
const int SEGMENTS_TOTAL =  7 * DIGIT_COUNT;

//Number of total leds for all digits
const int LED_COUNT = 7 * SEGMENT_COUNT * DIGIT_COUNT;

//For creating numbers in display
int LED_SEGMENTS[SEGMENTS_TOTAL][SEGMENT_COUNT] = {0};
int lsIndex = 0;


//All leds of all digits display connected
CRGB digitLeds[LED_COUNT];


//We create our colors by addjusting the RGB values of the LED

CRGB red = CRGB(255, 0, 0);
CRGB green = CRGB(0, 255, 0);
CRGB blue = CRGB(0, 0, 255);
CRGB cyan = CRGB(0, 255, 255);

//We can also create a palette if we want
CRGB colors[] = {CRGB(255, 0, 0),
                 CRGB(0, 255, 0),
                 CRGB(0, 0, 255),
                 CRGB(255, 255, 0),
                 CRGB(255, 0, 255),
                 CRGB(0, 255, 255),
                 CRGB(255, 255, 255),
                 CRGB(180, 100, 0),
                 CRGB(100, 100, 0),
                 CRGB(180, 100, 180),
                };

//Or even define a color dyanmically.
CRGB currentColor;

//Then we can define the values later
byte redVal = 255;
byte greenVal = 0;
byte blueVal = 0;

//This will be the number we display
int currentNumber = 0;


void setup() {

  //For debugging
  Serial.begin(9600);

  //Create the FastLED instance
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(digitLeds, LED_COUNT);

  //Can set the brightness between 0 and 255
  FastLED.setBrightness(100);

  //Create the array of leds used to turn on each segment
  for (int i = 0; i < SEGMENTS_TOTAL; i++ ) {
    for (int j = 0; j < SEGMENT_COUNT; j++) {
      LED_SEGMENTS[i][j] = lsIndex;
      lsIndex += 1;
    }
  }

}



void loop() {

  //Simple script to cycle through the numbers 0 - 9 while fading through all the colors

  //Cycle through each color value
  if (redVal > 0 && blueVal == 0) {
    redVal -= 5;
    greenVal += 5;
  }

  if (greenVal > 0 && redVal == 0) {
    greenVal -= 5;
    blueVal += 5;
  }

  if (blueVal > 0 && greenVal == 0) {
    blueVal -= 5;
    redVal += 5;
  }

  //Print each color value
  Serial.print(redVal);
  Serial.print("  ");
  Serial.print(greenVal);
  Serial.print("  ");
  Serial.println(blueVal);

  //Assign each value
  currentColor = CRGB(redVal, greenVal, blueVal);

  //Display the number with the color
  showNumber(currentNumber, currentColor);

  //Count
  currentNumber += 1;

  //The max number is based on the number of digits you have connecected.
  //With a single digit, the max is 9, with two digits, 99, etc.
  if (currentNumber >= 9) {
    currentNumber = 0;
  }

  //Update the Fast LED instance
  FastLED.show();

  //Wait a bit just to make reading the digit easier
  delay(250);

}


//Assisgns the number and color to a digit in the chain
//It will display the number from highest decimal place to lowest

void showNumber(int number, CRGB numberColor) {

  //Get the decimal place of the current number being displayed
  for (byte decPlace = 1; decPlace <= DIGIT_COUNT; decPlace++) {
    int remainder = number % 10;
    displayDigit(DIGIT_COUNT - decPlace, remainder, numberColor);
    number /= 10;
  }

}

//Controls the digits segments to display the character
void displayDigit(byte digitNum, byte number, CRGB displayColor ) {

  byte a = 1 << 6;
  byte b = 1 << 5;
  byte c = 1 << 4;
  byte d = 1 << 3;
  byte e = 1 << 2;
  byte f = 1 << 1;
  byte g = 1 << 0;

  byte numSegments;

  switch (number) {

    case 0:
      numSegments = a | b | c | e | f | g;
      break;

    case 1:
      numSegments = c | g;
      break;

    case 2:
      numSegments =  b | c | d | e | f;
      break;

    case 3:
      numSegments = b | c | d | f | g;
      break;

    case 4:
      numSegments = a | c | d | g;
      break;

    case 5:
      numSegments = a | b | d | f | g;
      break;

    case 6:
      numSegments =  a | b | d | e | f | g;
      break;

    case 7:
      numSegments = b | c | g;
      break;

    case 8:
      numSegments = a | b | c | d | e | f | g;
      break;

    case 9:
      numSegments = a | b | c | d | g;
      break;
  }


  for (byte ledSegment = 0; ledSegment < 7; ledSegment++) {
    if ((numSegments & 1 << (6 - ledSegment)) == 0) {
      segmentOff(ledSegment + 7 * digitNum);
    } else {
      segmentOn(ledSegment + 7 * digitNum, displayColor);
    }
  }
}


void segmentOn(int segmentNum, CRGB onColor) {
  for (int i = 0; i < SEGMENT_COUNT; i++) {
    digitLeds[LED_SEGMENTS[segmentNum][i]] = onColor;
  }
}

void segmentOff(int segmentNum) {
  for (int i = 0; i < SEGMENT_COUNT; i++) {
    digitLeds[LED_SEGMENTS[segmentNum][i]] = CRGB(0, 0, 0);
  }
}
