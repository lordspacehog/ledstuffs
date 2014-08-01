
#include <FastSPI_LED.h>

#define LEDS 100

uint32_t rng_fast();

static uint32_t rng_fast_seed = 1234;
static uint32_t dead_beef = 0xdeadbeef;

//struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *ledstrip;

int LEDarray[100];


CRGB colorarray[] = { 0x000080,0x0000CD,0x0000FF,0x00BFFF,0x00FFFF,0x00FA9A,0x00FF00,0x00FF7F,0xFF00FF,0xFF1493,0xFF4500,0xFF6347,0xFFBB00,0xFF8C00,0xFFA500,0xFFFF00};

void setup() {
  
  FastSPI_LED.setLeds(LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);
  FastSPI_LED.setDataRate(1);
  FastSPI_LED.init();
  FastSPI_LED.start();
  ledstrip = (struct CRGB*)FastSPI_LED.getRGBData();
}

uint32_t rng_fast() {
    rng_fast_seed = (rng_fast_seed << 7) ^ ((rng_fast_seed >> 25) + dead_beef);
    dead_beef = (dead_beef << 7) ^ ((dead_beef >> 25) + 0xdeadbeef);
    return rng_fast_seed;  
}

void loop () {
  uint32_t randnum = rng_fast();
  
  memset(ledstrip, 0, LEDS * 3);
     
  for(int i = 0 ; i < 25 ; i++) {
    LEDarray[i]=randnum & (1 << 4)-1;
    randnum >>= 1;
  }
  
  randnum = rng_fast();
  
  for(int i = 25 ; i < 50 ; i++) {
    LEDarray[i]=randnum & (1 << 4)-1;
    randnum >>= 1;
  }
  
  randnum = rng_fast();
  
  for(int i = 50 ; i < 75 ; i++) {
    LEDarray[i]=randnum & (1 << 4)-1;
    randnum >>= 1;
  }
  
  randnum = rng_fast();
  
  for(int i = 75 ; i < 100 ; i++) {
    LEDarray[i]=randnum & (1 << 4)-1;
    randnum >>= 1;
  }
  
  for(int i = 0 ; i < LEDS ; i++) {
    ledstrip[i]=colorarray[LEDarray[i]];
  }

  FastSPI_LED.show();
  
  delay(500);

}  
   
