uint32_t hours=0, minutes=0, seconds=0;
uint32_t phours=0, pminutes=0, pseconds=0;

uint32_t ihours=23, iminutes=30, iseconds=10;

unsigned long old_millis=0, new_millis=0;

uint16_t p=1000; //ponderator use less than 1000 to test
void setup()
{
        Serial.begin(9600);

}

void loop()
{
        old_millis=millis();
        delay(p);
        new_millis=millis();
        if ( old_millis > new_millis ) {
         ihours=hours;
         iminutes=minutes;
         iseconds=seconds;
        }

        hours=millis()/p/60/60 + ihours; 
        minutes=millis()/p/60 + iminutes;  
        seconds=millis()/p  + iseconds;
    
        
        if (minutes >= 1) {

                pseconds=seconds%60;
        } else {         
                pseconds=seconds;
        }
        
        if (hours >= 1) {

                pminutes=minutes%60;
        } else {  
                pminutes=minutes;
        }   
        
        if (hours >= 24) {
                phours=hours%24;
        } else {  
                phours=hours;          
        }       
        
        Serial.print(phours,DEC);
        Serial.print(":");            
        Serial.print(pminutes,DEC);
        Serial.print(":");
        Serial.println(pseconds,DEC);
}

