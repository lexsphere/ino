#include <Adafruit_NeoPixel.h>

#define PIN            12
#define NUMPIXELS      12
#define BPIN_1         10
#define BPIN_2         9

#include <Wire.h>
#include "RTClib.h"
#include "PinChangeInterrupt.h"
#include <EEPROM.h>

RTC_DS1307 RTC;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint8_t delayval = 500;
uint8_t hour=0;
uint8_t minute=0;
uint8_t hpixel=0;
uint8_t mpixel=0;
uint8_t sec1=0;
uint8_t sec2=0;
uint8_t b1_counter=0;
uint8_t b2_counter=0;
bool oldState1 = HIGH;
bool oldState2 = HIGH;
bool newState1 = HIGH;
bool newState2 = HIGH;
uint8_t bmode1=0;
uint8_t bmode2=0;
uint8_t status=0;
uint8_t status2=0;
uint8_t status3=0;
bool eflag = false;
uint8_t timer_val=0;
uint8_t timeout1=3;
uint8_t timeout2=8;
int addr = 0;

void setup() {
        pixels.begin();
        Serial.begin(57600);
        Wire.begin();
        RTC.begin();
    
        if(Serial)
                RTC.adjust(DateTime(__DATE__, __TIME__)); /* set PC date */
 
        pinMode(BPIN_1, INPUT_PULLUP);
        pinMode(BPIN_2, INPUT_PULLUP);

        attachPCINT(digitalPinToPCINT(BPIN_2), b2interr, FALLING);
        attachPCINT(digitalPinToPCINT(BPIN_1), b1interr, FALLING);
}

void loop() {
        DateTime now = RTC.now();
        hour=now.hour()%12;
        minute=now.minute();
        hpixel=(hour+10)%12;
        mpixel=(minute/5+10)%12;
        sec1=now.second();
        Serial.print(hour, DEC);
        Serial.print(':');
        Serial.print(minute, DEC);
        Serial.print(':');
        Serial.print(sec1, DEC);
        Serial.println();
        Serial.println("read val from EEPROM");
        Serial.println(EEPROM.read(addr));

        //set_color(0,0,0);

        if (timer_val > 0) {
                handle_timer(timer_val);
        }
        
        show_clock(hpixel, mpixel);
        delay(1000);

}

void b2interr() {
        Serial.println("D9 - B2 interrupt");
        eflag = false;
        EEPROM.write(addr, sec1);
}
void handle_timer(uint8_t tval) {

        uint8_t nos=0; /* no of seconds */
        
        if (tval == 1) {
                Serial.println("timer_val=1");
                set_npixels(1,0,0,1);
                while (nos < timeout1) {
                        nos++;
                        set_npixels(1,0,0,1);
                        delay(1000);
                        set_color(0,0,0);
                }
                eflag = true;
                flash(100,0,0,255);
        }
        if (tval == 2) {
                Serial.println("timer_val=2");
                set_npixels(1,0,0,1);
                while (nos < timeout2) {
                        nos++;
                        set_npixels(1,1,0,0);
                        delay(1000);
                        set_color(0,0,0);
                }
                eflag = true;
                flash(100,255,0,0);

        }
        delay(50);
        set_color(0,0,0);

}
void b1interr() {
        Serial.println("D10 -B1 interrupt");
        detachPCINT(digitalPinToPCINT(BPIN_2));
        b1_counter++;
        Serial.println(b1_counter,DEC);
        eflag=true; 
        if (b1_counter > 1) {
                b1_counter = 0;
                eflag = false;
                Serial.println("eflag is:");
                Serial.println(eflag,BIN);
                attachPCINT(digitalPinToPCINT(BPIN_2), b2interr, FALLING);
        }
        set_color(0,1,0);
        while(eflag) {
                Serial.println("eflag is:");
                Serial.println(eflag,BIN);
                while ( digitalRead(BPIN_2) == HIGH && eflag) {
                        delay(50);
                        if ( digitalRead(BPIN_2) == LOW ) {
                                b2_counter++;
                                break;
                        }
                }
                if ( b2_counter == 1) {
                        set_npixels(1,0,0,1);
                        timer_val=1;
                }
                if ( b2_counter == 2) {
                        set_npixels(2,1,0,0);
                        timer_val=2;
                }
                if ( b2_counter > 1 ) {
                        b2_counter=0;
                }
                delay(100);
                if ( digitalRead(BPIN_1) == LOW ) {
                        break;
                }
        }
}
void show_clock(uint8_t hpixel, uint8_t mpixel) {
        for(uint8_t i=0;i<NUMPIXELS;i++)
                set_pixels(i, hpixel, mpixel);
}
void set_pixels(uint8_t i, uint8_t hpixel, uint8_t mpixel) {
        uint8_t r=0, g=0, b=0;

        if (i == hpixel || i == mpixel ) {
            if (i == hpixel)
                    r = 1; 
            if (i == mpixel) 
                    b = 1;
        }
                
        pixels.setPixelColor(i, pixels.Color(r,g,b));
        pixels.show();
}

void set_npixels(uint8_t nopix,uint8_t r, uint8_t g, uint8_t b) {
        set_color(0,0,0);
        if (nopix == 1) {        
               pixels.setPixelColor(0, pixels.Color(r,g,b));
               pixels.setPixelColor(4, pixels.Color(r,g,b));
               pixels.setPixelColor(8, pixels.Color(r,g,b));
        }
        if (nopix == 2) {        
               pixels.setPixelColor(2, pixels.Color(r,g,b));
               pixels.setPixelColor(6, pixels.Color(r,g,b));
               pixels.setPixelColor(10, pixels.Color(r,g,b));
        }
        pixels.show();
}

void flash(uint8_t max, uint8_t r, uint8_t g, uint8_t b) {
        timer_val = 0;
        for(uint8_t i=0;i<max;i++){
                Serial.println("flash: eflag is:");
                Serial.println(eflag,BIN);
                if ( eflag == false )
                        break;
                set_color(r,g,b); 
                delay(50);
                set_color(0,0,0); 
                delay(500);
        }

}
void set_color(uint8_t r, uint8_t g, uint8_t b) {
        for(uint8_t i=0;i<NUMPIXELS;i++){
                pixels.setPixelColor(i, pixels.Color(r,g,b));
                pixels.show();
        }

}
void set_all_pixels(uint8_t color) {
        for(uint8_t i=0;i<NUMPIXELS;i++){
                pixels.setPixelColor(i, pixels.Color(color,color,color));
        }
        pixels.show();
}
uint8_t get_button_mode(uint8_t pin, uint8_t dly) {

        uint8_t i = 0;
        uint8_t retval=0;
        bool state1 = digitalRead(pin);
        delay(dly);
        bool state2 = digitalRead(pin);
        if ( state1 == LOW && state2 == LOW ) {
                retval = 2;
        } else if ( state1 == LOW && state2 == HIGH ) {
                retval = 1;              
        } else if ( state1 == HIGH && state2 == HIGH ) {
                retval = 0;
        }

        return retval;
}
