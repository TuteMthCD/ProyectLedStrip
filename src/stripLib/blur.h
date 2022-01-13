#include <Arduino.h>

void blur_effect1(void);
void blur_effect2(void);

class blur {
private:
  int position=0;
  bool direction=0;     //0 lef ; 1 right
  unsigned int color;
public:
  blur(void);
  void move1(void);
  void move2(void);
};
