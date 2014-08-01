#include <FastSPI_LED.h>


#define NUM_LEDS 100

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
  unsigned char b; 
  unsigned char r; 
  unsigned char g; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

uint32_t Color(byte r, byte g, byte b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}


uint32_t Wheel(uint16_t WheelPos)
{
  int r, g, b;
  uint32_t retcolor;
  switch(WheelPos / 255)
  {
    case 0:
      r = 254 - WheelPos % 255; // red down
      g = WheelPos % 255;       // green up
      b = 0;                    // blue off
      break;
    case 1:
      g = 254 - WheelPos % 255; // green down
      b = WheelPos % 255;       // blue up
      r = 0;                    // red off
      break;
    case 2:
      b = 254 - WheelPos % 255; // blue down
      r = WheelPos % 255;       // red up
      g = 0;                    // green off
      break;
  }
  
  retcolor = Color( r, g, b);
  
  return(retcolor);
}

// Cycle through the color wheel, equally spaced around the belt
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  uint32_t pxcolor;

  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < NUM_LEDS; i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color
      // wheel (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      pxcolor = Wheel(((i * 384 / NUM_LEDS) + j) % 384);
      leds[i].r = (pxcolor>>16)&255;
      leds[i].g = (pxcolor>>8)&255; 
      leds[i].b = pxcolor&255;
    }
    FastSPI_LED.show();   // write all the pixels out
    delay(wait);
  }
}




// --------------------------------------------
//      setup
// --------------------------------------------
void setup() {

  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);
//  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);

  //select spi speed, 7 is very slow, 0 is blazing fast
  //hint: the small (1 led, 5v) spi modules can run maximal at speed2!
  FastSPI_LED.setDataRate(1);
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

// --------------------------------------------
//      main loop
// --------------------------------------------
void loop() {
  memset (leds, 0, NUM_LEDS *3);
  
  rainbowCycle(5);	
}
