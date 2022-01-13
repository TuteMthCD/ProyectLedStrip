#include "striplib.h"
#include "blur.h"
#include <FastLED.h>

#define DATA_PIN 5

#define TIME		   5
#define DEFAULT_BRIGHTNESS 10

CRGB leds[NUM_LEDS];
LedStrip strip;

unsigned long timer_0 = 0;
int i		      = 0;
// tatic bool prev_light_state;

void LedStrip::init(void) {
	FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
	FastLED.setBrightness(DEFAULT_BRIGHTNESS);
	FastLED.clear();
	color = CRGB::White;
	// for(char i=0;i<NUM_LEDS;i++)leds[i]=color;
	FastLED.show();
}

void LedStrip::setcolor(unsigned int incolor) {
	color = incolor;
	if (light_state) {
		light_change = 1;
		i	     = 0;
		while (i < NUM_LEDS) {
			leds[i] = color;
			i++;
		}
		FastLED.show();
	}
}
void LedStrip::setBrightness(unsigned char brightness) {
	brightness = map(brightness, 0, 100, 0, 255);
	FastLED.setBrightness(constrain(brightness, 1, 255));
	FastLED.show();
	// light_change = 1;
}

void LedStrip::setEffect(unsigned char effectselect) {
	light_effect = effectselect;
	light_change = 1;
	i	     = 0;
}
void LedStrip::setVelocity(unsigned char Velocityselect) {

	light_velocity =(unsigned int)map(pow(Velocityselect,3/2),0,1000,3,150);
}

void LedStrip::light(bool status) {
	light_change = 1;
	i	     = 0;
	light_state  = status;
}

#define TIME_OPEN 15

void LedStrip::handle() {
	// on with LUCES ON/OFF
	if (light_change) {
		switch (light_effect) {
		case 0:
			effect_0();
			break;
		case 1:
			effect_1();
			break;
		case 2:
			effect_2();
			break;
		case 3:
			effect_3();
			break;
		case 4:
			effect_4();
			break;
		case 5:
			effect_5();
			break;
		case 6:
			effect_6();
			break;
		case 7:
			effect_7();
			break;
		case 255:
			ESP.restart(); // reset state
			break;
		default:
			close();
		}
	}
	//--------------------------END
}

void strip_init(void) { strip.init(); }

void strip_handle(void) { strip.handle(); }

void showProgres(int curr, int total) {
	int progress = map(curr, 0, total, 0, NUM_LEDS);
	for (int i = 0; i < NUM_LEDS; i++) {
		if (i < progress)
			leds[i] = CRGB::Green;
		else
			leds[i] = CRGB::Black;
	}
	FastLED.show();
}

// effects
void LedStrip::close(void) {
	if (timer_0 + TIME_OPEN < millis()) {
		asm("nop");
		asm("nop");
		timer_0 = millis();
		if (i < NUM_LEDS) {
			leds[i] = CRGB::Black;
			i++;
		} else {
			light_change = 0;
			i	     = 0;
		}
		FastLED.show();
	}
}

void LedStrip::effect_0(void) {
	if (light_state) {
		if (timer_0 + TIME_OPEN < millis()) {
			timer_0 = millis();
			if (i < NUM_LEDS) {
				leds[i] = color;
				i++;
			} else {
				light_change = 0;
				i	     = 0;
			}
			FastLED.show();
		}
	} else
		close();
}

void LedStrip::effect_1(void) {
	if (light_state) {
		if (timer_0 + TIME_OPEN < millis()) {
			timer_0 = millis();
			if (i < NUM_LEDS / 2) {
				leds[(NUM_LEDS / 2) + i]       = color;
				leds[(NUM_LEDS / 2) - (i + 1)] = color;
				i++;
			} else {
				light_change = 0;
				i	     = 0;
			}
			FastLED.show();
		}
	} else {
		if (timer_0 + TIME_OPEN < millis()) {
			timer_0 = millis();
			if (i < NUM_LEDS / 2) {
				leds[(NUM_LEDS / 2) + i]       = CRGB::Black;
				leds[(NUM_LEDS / 2) - (i + 1)] = CRGB::Black;
				i++;
			} else {
				light_change = 0;
				i	     = 0;
			}
			FastLED.show();
		}
	}
}

void LedStrip::effect_2(void) {
	if (light_state) {
		if (timer_0 + light_velocity < millis()) {

			static int effect_color = color;
			static bool step	= 1;

			if (i < NUM_LEDS) {
				if (step)
					leds[i] = CHSV(effect_color, 255, 255);
				else
					leds[(NUM_LEDS - 1) - i] =
					    CHSV(effect_color, 255, 255);
				i++;
			} else {
				step	     = !step;
				effect_color = random(0, 255);
				i	     = 0;
			}

			timer_0 = millis();
			FastLED.show();
		}
	} else
		close();
}
void LedStrip::effect_3(void) {
	if (light_state) {
		if (timer_0 + light_velocity < millis()) {
			leds[random(0, NUM_LEDS)] =
			    CHSV(random(0, 255), 255, 255);
			leds[random(0, NUM_LEDS)] =
			    CHSV(random(0, 255), 255, 255);
			leds[random(0, NUM_LEDS)] =
			    CHSV(random(0, 255), 255, 255);
			timer_0 = millis();
			FastLED.show();
		}
	} else
		close();
}

void LedStrip::effect_4(void) {
	if (light_state) {
		if (timer_0 + light_velocity < millis()) {
			static int effect_color = random(0, 255);
			if (i < NUM_LEDS / 4) {
				leds[(0 * NUM_LEDS / 4) + i] =
				    CHSV(effect_color, 255, 255);
				leds[(1 * NUM_LEDS / 4) + i] =
				    CHSV(effect_color, 255, 255);
				leds[(2 * NUM_LEDS / 4) + i] =
				    CHSV(effect_color, 255, 255);
				leds[(3 * NUM_LEDS / 4) + i] =
				    CHSV(effect_color, 255, 255);
				i++;
				timer_0 = millis();
				FastLED.show();
			} else {
				i	     = 0;
				effect_color = random(0, 255);
			}
		}
	} else
		close();
}

void LedStrip::effect_5(void) {
	if (light_state) {
		// for(int i=0; i < NUM_LEDS; i++)
		// leds[i]=CHSV(map(i,0,NUM_LEDS,0,255), 255, 255);
		if (timer_0 + light_velocity < millis()) {
			if (i < NUM_LEDS) {
				leds[i] =
				    CHSV(map(i, 0, NUM_LEDS, 0, 255), 255, 255);
				i++;
			} else {
				i	     = 0;
				light_change = 0;
			}
			// light_change=0;
			timer_0 = millis();
			FastLED.show();
		}
	} else
		close();
}

void LedStrip::effect_6(void) {
	if (light_state) {
		if (timer_0 + light_velocity < millis()) {
			blur_effect1();
			timer_0 = millis();
			FastLED.show();
		}
	} else
		close();
}

void LedStrip::effect_7(void) {
	if (light_state) {
		if (timer_0 + light_velocity < millis()) {
			blur_effect2();
			timer_0 = millis();
			FastLED.show();
		}
	} else
		close();
}
/*----------------------------------------------------------------------------*/
