#include <Adafruit_GPS.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
Adafruit_GPS GPS(&Serial1);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6, NEO_GRB + NEO_KHZ800);

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
mdate date1 = { 15 , 11 , 20 , 19 , 12 , "taxi" };
mdate date2 = { 15 , 11 , 21 , 17 , 44 , "new" };
mdate *pdates[]={&date1,&date2};

void setup()
{
        Serial.begin(115200);       
        GPS.begin(9600);
        GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate     
        GPS.sendCommand(PGCMD_ANTENNA);
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
        //delay(8000);      

}

uint32_t gpsTimer = millis();
uint32_t startupTimer = millis();
uint32_t compassTimer = millis();
uint8_t tnevents; //  number of events

void loop() // run over and over again
{
        char c = GPS.read();        
        GPS.parse(GPS.lastNMEA());

        if ( millis() % 2000 == 0 ) { 
                //Serial.println(sizeof(pdates));
                tnevents=0;
                for ( int i = 0; i < sizeof(pdates)/2; i++ ) {
                        //Serial.print("pdates::");Serial.print(i);Serial.print("::");
                        if ( pdates[i]->year == GPS.year && pdates[i]->month == GPS.month && pdates[i]->day == GPS.day && pdates[i]->hour == GPS.hour && pdates[i]->minute == GPS.minute) {
                                //Serial.println(pdates[i]->msg);
                                strobe();                                    
                        }
                        // here you should have an if; if you did not press the button the task goes into the queue
                        Serial.print(GPS.day);Serial.print("::");Serial.print(pdates[i]->day);
                        if ( (pdates[i]->year < GPS.year) ) tnevents++; // incr. if year is overdue 
                        if (pdates[i]->year == GPS.year)  {
                                if (pdates[i]->month < GPS.month) tnevents++; //month is overdue
                                else if (pdates[i]->month == GPS.month) {
                                        if (pdates[i]->day < GPS.day) tnevents++; //day is overdue
                                        else if (pdates[i]->day == GPS.day) {
                                                if (pdates[i]->hour < GPS.hour) tnevents++; //hour is overdue 
                                                else if (pdates[i]->hour == GPS.hour) {
                                                        if (pdates[i]->minute < GPS.minute) tnevents++; //minute is overdue
                                                }
                                        }
                                }
                        }                        
                        lcounter(tnevents);                          
                }
                        
        }

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
                      strip.setPixelColor(i, strip.Color(30, 0, 0));
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
            strip.setPixelColor(i, strip.Color(2, 0, 0));
            strip.show();
            delay(10);
      }
}
