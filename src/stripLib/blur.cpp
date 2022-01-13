#include "blur.h"
#include "striplib.h"
#include <FastLED.h>

#define NUM_BLUR 20

extern CRGB leds[NUM_LEDS];

blur blur[NUM_BLUR];

blur::blur(void){
        position = random(0,NUM_LEDS);
        if (random(0,255) > 127) direction = 1;
        else direction=0;
        color=random(0,255);
}

void blur::move1(void){
        if(direction) {
                if(position<NUM_LEDS && position>0) leds[position]=CHSV(color, 255, 255);
                if((position-1)<NUM_LEDS && (position-1) > 0) leds[position-1]=CHSV(color, 255, 255/2);
                if((position-2)<NUM_LEDS && (position-2) > 0) leds[position-2]=CHSV(color, 255, 255/4);
                if((position-3)<NUM_LEDS && (position-3) > 0) leds[position-3]=CRGB::Black;
                if(position<(NUM_LEDS+3)) position++;
                else {
                        position = random(0,NUM_LEDS);
                        if (random(0,255) > 127) direction = 1;
                        else direction=0;
                        color=random(0,255);
                }

        }else{
                if(position<NUM_LEDS && position>0) leds[position]=CHSV(color, 255, 255);
                if((position+1)<NUM_LEDS && (position+1)>0) leds[position+1]=CHSV(color, 255, 255/2);
                if((position+2)<NUM_LEDS && (position+2)>0) leds[position+2]=CHSV(color, 255, 255/4);
                if((position+3)<NUM_LEDS && (position+3)>0) leds[position+3]=CRGB::Black;
                if(position > (-3)) position--;
                else {
                        position = random(0,NUM_LEDS);
                        if (random(0,255) > 127) direction = 1;
                        else direction=0;
                        color=random(0,255);
                }
        }
}

void blur_effect1(void){
        for(unsigned char i=0; i<NUM_BLUR; i++) blur[i].move1();
}

void blur::move2(void){
        if(random(0,100)>90) {
                if (random(0,100) > 90) direction = 1;
                else direction=0;
                color=random(0,255);
        }
        if(direction) {
                if(position<NUM_LEDS && position>0) leds[position]=CHSV(color, 255, 255);
                if((position-1)<NUM_LEDS && (position-1) > 0) leds[position-1]=CHSV(color, 255, 255/2);
                if((position-2)<NUM_LEDS && (position-2) > 0) leds[position-2]=CHSV(color, 255, 255/4);
                if((position-3)<NUM_LEDS && (position-3) > 0) leds[position-3]=CRGB::Black;
                if(position<(NUM_LEDS+3)) position++;
                else position=0;

        }else{
                if(position<NUM_LEDS && position>0) leds[position]=CHSV(color, 255, 255);
                if((position+1)<NUM_LEDS && (position+1)>0) leds[position+1]=CHSV(color, 255, 255/2);
                if((position+2)<NUM_LEDS && (position+2)>0) leds[position+2]=CHSV(color, 255, 255/4);
                if((position+3)<NUM_LEDS && (position+3)>0) leds[position+3]=CRGB::Black;
                if(position > (-3)) position--;
                else position=NUM_LEDS-1;
        }
}
void blur_effect2(void){
        blur[1].move2();
}
