#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#define BUTTON_PIN1   12    
#define BUTTON_PIN2  10
#define BUTTON_PIN3  9
#define PIXEL_PIN    6    
#define PIXEL_COUNT 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
bool oldState = HIGH;
int showType = 0;
uint16_t lint=0;
uint16_t lstep=1; // lstep - is the light step
uint16_t r=0, g=0, b=0, bpit=0; //bpit - button_pin itteration
uint16_t rp=1, gp=1, bp=1; //color ponderers
uint16_t dlevel=-1;
void setup() {
        Serial.begin(9600);
        pinMode(BUTTON_PIN1, INPUT_PULLUP);
        pinMode(BUTTON_PIN2, INPUT_PULLUP);
        pinMode(BUTTON_PIN3, INPUT_PULLUP);
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
}
void loop() {

        set_color( BUTTON_PIN3 );
        set_intensity( BUTTON_PIN1, 10 ,5 );
        set_intensity( BUTTON_PIN2, 1 ,100 );

}
void set_color(uint16_t button_pin) {
        bool newState = digitalRead(button_pin);
        if (newState == LOW && oldState == HIGH) {
                delay(1);
                newState = digitalRead(button_pin);
                if (newState == LOW) {  
                        if (bpit > 4) {
                                bpit=0;
                        }
                        switch(bpit) {
                                case 0: rp=1; gp=0; bp=0; //red
                                break;  
                                case 2: rp=0; gp=0; bp=1; //blue
                                break; 
                                case 3: rp=1; gp=0; bp=1; //purple
                                break;    
                                case 1: rp=0; gp=1; bp=0; //green
                                break;                                               
                                case 4: rp=1; gp=1; bp=1; //white
                                break;   
                        }
                        bpit++;
                        r=lint*rp; g=lint*gp; b=lint*bp;                                
                        changeli(r,g,b,0);
                }
          }  
}
void set_intensity(uint16_t button_pin, uint16_t lstep, uint16_t range) {
        bool newState = digitalRead(button_pin);
        if (newState == LOW && oldState == HIGH) {
                delay(1);
                newState = digitalRead(button_pin);
                if (newState == LOW) { 
                          dlevel++;                                               
                          if ((dlevel > range) || (dlevel <= 0)) {
                                  lint=0;
                                  dlevel=0;
                          } else {
                                  lint+=lstep;                          
                          }
                          //Serial.println(dlevel,DEC);
                          if (dlevel == range) {
                                  lint=255;
                                  dlevel=-1;
                          }                        
                          r=lint*rp; g=lint*gp; b=lint*bp;                                
                          changeli(r,g,b,0);                                      
                }
        }
}
void changeli(uint16_t r, uint16_t g, uint16_t b, uint8_t wait) { //change light intensity
      uint16_t i, j;
      //lint - is light intensity      
      for(j=0; j<256; j++) {
              for(i=0; i<strip.numPixels(); i++) {
                      strip.setPixelColor(i,strip.Color(r, g, b));
              }
              strip.show();
              delay(wait);
      }  
}
