#include <Arduino.h>

#define NUM_LEDS 100

class LedStrip{
  public:
    void init(void);
    void setcolor(unsigned int); //setea color
    void setBrightness(unsigned char);  //setea brillo
    void setEffect(unsigned char);  //setea effecto
    void setVelocity(unsigned char); //setea velocidad (tiempo entre cada ciclo)

    void light(bool); //enciende o apaga 0 y 1
    void handle(void);
  protected:
    void close(void);

    void effect_0(void);
    void effect_1(void);
    void effect_2(void);
    void effect_3(void);
    void effect_4(void);
    void effect_5(void);
    void effect_6(void);
    void effect_7(void);

    unsigned char light_effect=0;
    unsigned int color=0xFFFFFF;
    bool light_change=0;
    bool light_state=0;

    unsigned int light_velocity=15;
};

extern LedStrip strip;

void strip_init(void);
void strip_handle(void);
void showProgres(int, int);
