#include <Adafruit_GPS.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
Adafruit_GPS GPS(&Serial1);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ800);
#define BUTTON_PIN2 10
bool oldState = HIGH;

struct mdate
{
        int year;
        int month;
        int day;
        int hour;
        int minute;
        String msg;         
};
//mdate  d0 = { yy , mm , dd , hh , mm , "example" };
//this is just a comment
mdate date1 = { 15 , 11 , 27 , 19 , 12 , "taxi" };
mdate date2 = { 15 , 11 , 27 , 17 , 44 , "new" };
mdate *pdates[]={&date1,&date2};

void setup()
{

        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
        //delay(8000);    
        pinMode(BUTTON_PIN2, INPUT_PULLUP);  

}

uint32_t gpsTimer = millis();
uint32_t startupTimer = millis();
uint32_t compassTimer = millis();
uint8_t tnevents; //  number of events
uint8_t pi=0;
uint8_t color=0;
void loop() // run over and over again
{
 

        
        for(int i=0; i<16; i++) {
              lightup(i,color);
              delay(50);
              //lightdown(i);
        }
        
        color++;
        if( color > 2 ) {
              color=0;
        }
        

        
}

void lightup(uint8_t pixel, uint8_t color) {
  
        uint8_t r,g,b,sty=2;
      
        switch(color) {        
                 case 2:
                    r=sty;g=0;b=0;
                    break;               	
                 case 4:
                    r=0;g=sty;b=0;
                    break;                 
                 case 0:
                   r=0;g=0;b=sty; 
                 break;
                 case 3:
                   r=sty;g=0;b=sty; 
                 break;  
                 case 1:
                   r=0;g=sty;b=sty; 
                 break;               
                 
        }


        strip.setPixelColor(pixel, strip.Color(r, g, b));
        strip.show();
}
void lightdown(uint8_t pixel) {
        strip.setPixelColor(pixel, strip.Color(0, 0, 0));
        strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, c);
            strip.show();
            delay(wait);
      }
}
void strobe() {
      for(int j=0; j<20; j++){
              for(uint16_t i=0; i<strip.numPixels(); i++) {
                      strip.setPixelColor(i, strip.Color(0, 0, 255));
                      strip.show();
              }
              delay(20);
              colorWipe(strip.Color(0, 0, 0), 0);
              delay(1000);
      }
}
void lcounter(uint8_t npix) {
  // npix = number of pixels to lit
      for(uint16_t i=0; i<npix; i++) {
            strip.setPixelColor(i-1, strip.Color(0, 0, 0));
            strip.show();
            strip.setPixelColor(i, strip.Color(2, 0, 0));
            strip.show();
            delay(10);
      }
      //colorWipe(strip.Color(0, 0, 0), 0);
}
void lcounter2(uint8_t npix) {
  // npix = number of pixels to lit
      //for(uint16_t i=0; i<npix; i++) {
            strip.setPixelColor(npix, strip.Color(random(0, 2), random(0, 2), random(0, 2)));
            strip.show();
            delay(10);
      //}
}
void lcounter3(uint8_t npix) {
  // npix = number of pixels to lit
      //for(uint16_t i=0; i<npix; i++) {
            npix = npix % 17;
            strip.setPixelColor(npix-1, strip.Color(0, 0, 0));
            strip.setPixelColor(npix, strip.Color(random(0, random(10)), random(0, random(10)), random(0, random(10))));
            strip.show();
            delay(10);
      //}
}
