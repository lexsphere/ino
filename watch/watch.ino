#include <Wire.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ400);
int showType = 0;
uint32_t hour=0, minutes=0, seconds=0;
uint32_t ihour=, iminutes=0, iseconds=0;

uint8_t myFavoriteColors[][3] = {{200,   0, 200},   // purple
  {0,   117,   255},   // blue
  {200, 200, 200},   // white
};

#define FAVCOLORS sizeof(myFavoriteColors) / 3
#define MOVE_THRESHOLD 300

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  //Serial.println("Twinkle!");
  //flashRandom(240, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
  //flashRandom(30, 16);  // second number is how many neopixels to simultaneously light up
  //flashRandom(30, 2);
  //startShow(7);
  rainbow(30);
  //startShow(0);
}


void flashRandom(int wait, uint8_t howmany) {

  for (uint16_t i = 0; i < howmany; i++) {
    int c = random(FAVCOLORS);// pick a random favorite color!
    int red = myFavoriteColors[c][0];
    int green = myFavoriteColors[c][1];
    int blue = myFavoriteColors[c][2];
    
    int j = random(strip.numPixels()); // get a random pixel from the list
    //Serial.print("Lighting up "); Serial.println(j);
    
    for (int x = 0; x < 1; x++) { // now we will 'fade' it in 5 steps
      int r = red * (x + 1); r /= 5;
      int g = green * (x + 1); g /= 5;
      int b = blue * (x + 1); b /= 5;

      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }    
    for (int x = 1; x >= 0; x--) { // & fade out in 5 steps
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }
  } // LEDs will be off when done (they are faded to 0)  
}

void startShow(int i) {
  switch (i) {
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
      break;
    case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
      break;
    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
      break;
    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
      break;
    case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
      break;
    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
      break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
      break;
    case 7: rainbow(10);
      break;
    case 8: rainbowCycle(0);
      break;
    case 9: theaterChaseRainbow(50);
      break;
  }
}

void colorWipe(uint32_t c, uint8_t wait) { // Fill the dots one after the other with a color
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
        uint16_t i, j;

        for (j = 0; j < 2560; j++) {
                for (i = 0; i < strip.numPixels(); i++) {
                        
                        
                        strip.setPixelColor(i, Wheel((i + j) & 255));
                }
                strip.show();
                delay(1000);
                hour=millis()/1000/60/60;
                minutes=millis()/1000/60;
                if (minutes < 1) {
                        seconds=millis()/1000;
                } 
                else {
                        seconds=(millis()-(minutes*60*1000))/1000;
                }
                Serial.print(hour,DEC);
                Serial.print(":");            
                Serial.print(minutes,DEC);
                Serial.print(":");
                Serial.println(seconds,DEC);
        }
}

void rainbowCycle(uint8_t wait) { // Slightly different, this makes the rainbow equally distributed throughout
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    //delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) { //Theatre-style crawling lights.
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) { //Theatre-style crawling lights with rainbow effect
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) { // Input a value 0 to 255 to get a color value.
  WheelPos = 255 - WheelPos;    // The colours are a transition r - g - b - back to r.
  int p = 100;
  if (WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) / p, 0, (WheelPos * 3) / p);
    
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) / p, (255 - WheelPos * 3) / p);
    
  }
  WheelPos -= 170;
    return strip.Color((WheelPos * 3) / p, (255 - WheelPos * 3) / p, 0);
  
}
