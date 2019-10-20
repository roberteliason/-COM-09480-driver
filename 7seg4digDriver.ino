/**
 * Proof of concept code that drives a 
 * Sparkfun 4-digit 7-segment display. 
 * https://www.sparkfun.com/products/9480
 * 
 * Datasheet:
 * https://www.sparkfun.com/datasheets/Components/LED/7-Segment/YSD-439AY2B-35.pdf
 * 
 * Add 100 Ohm resistors to pins 9-12
 * or risk burning out segments.
 * 
 * The quirk with this display is that you
 * cannot address the digits individually,
 * they share the same 'data bus'.
 * So you have to flash them one by one,
 * super fast in order to fool the eye.
 */

// Dedicate the pins
const int SEG_A = 2;
const int SEG_B = 3;
const int SEG_C = 4;
const int SEG_D = 5;
const int SEG_E = 6;
const int SEG_F = 7;
const int SEG_G = 8;
const int ELM_1 = 9;
const int ELM_2 = 10;
const int ELM_3 = 11;
const int ELM_4 = 12;

// An element maps to a digit on the display
// Setting these pins high lights up the segments
// that are toggled for those elements.
int elements[4] = {
  ELM_1, ELM_2, ELM_3, ELM_4
};

// Enumerate the segments in each element
// Setting these pins LOW lights the segments up.
int segments[7] = {
  SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G
};

// A digit is the shape, or which segments
// in each elements is toggled LOW to light up
// to make the representation of a number.
// Warning: Mind****s;
int digits[10][7] = {
  { 0,0,0,0,0,0,1 }, // 0
  { 1,0,0,1,1,1,1 }, // 1
  { 0,0,1,0,0,1,0 }, // 2
  { 0,0,0,0,1,1,0 }, // 3
  { 1,0,0,1,1,0,0 }, // 4
  { 0,1,0,0,1,0,0 }, // 5
  { 0,1,0,0,0,0,0 }, // 6
  { 0,0,0,1,1,1,1 }, // 7
  { 0,0,0,0,0,0,0 }, // 8
  { 0,0,0,1,1,0,0 }, // 9
}; 

// The integer we want to display (0-9999) 
int number = 0;

// The individual digits in above number
// This will be automatically updated in the loop
int digitsInNumber[4] = {0,0,0,0};

/**
 * Loop through the pins we plan to use 
 * and set them accordingly
 */
void setup() {
  // init Segment pins
  for (int pin = SEG_A; pin <= SEG_G; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // init Element pins
  for (int pin = ELM_1; pin <= ELM_4; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

/**
 * Turn all elements off
 */
void disableElements() {
  for (int pin = ELM_1; pin <= ELM_4; pin++) {
    digitalWrite(pin, LOW);
  }
}

/**
 * Turn all segments off
 */
void blankSegments() {
    for (int seg = SEG_A; seg <= SEG_G; seg++) {
      digitalWrite(seg, HIGH);
    }
}

/**
 * Update the segments with the set number
 */
void setSegments(int value) {
  blankSegments();
  for (int seg = 0; seg <= 6; seg++) {
    digitalWrite(segments[seg], (digits[value][seg]==1) ? HIGH : LOW);
  }
}

/**
 * Update selected digit, enable
 */
void updateElement(int digit, int value) {
  setSegments(value);
  digitalWrite(digit, HIGH);
}


/**
 * Split the number into it's digit components
 * so that we can feed them one by one to the
 * matching elements.
 */
void mapNumberToDigits(int * digitsInNumber, int number) {
  digitsInNumber[0] = number / 1000 % 10;
  digitsInNumber[1] = number / 100 % 10;
  digitsInNumber[2] = number / 10 % 10;
  digitsInNumber[3] = number % 10;
} 


/**
 * You loop me right round, baby right round
 */
void loop() {
  number = millis() / 1000;
  mapNumberToDigits(digitsInNumber, number);

  // Loop through the elements one by one and 
  // flash the digit that corresponds to said
  // element. Really, rally, fast. POV!
  for (int dig = 0; dig <= 3; dig ++) {
    disableElements();
    updateElement( elements[dig], digitsInNumber[dig] );
 }
}
