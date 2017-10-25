#include "U8glib.h"
#define dino_width 21
#define dino_height 20
#define smallcactus_width 8
#define smallcactus_height 17

static unsigned char smallcactus_bits[] = {
   0x18, 0x18, 0x98, 0x98, 0x99, 0x99, 0x99, 0x99, 0x79, 0x19, 0x1e, 0x18,
   0x18, 0x18, 0x18, 0x18, 0x18 };

static unsigned char dino_bits[] = {
   0x00, 0xf0, 0x0f, 0x00, 0xd8, 0x1f, 0x00, 0xf8, 0x1f, 0x00, 0xf8, 0x1f,
   0x00, 0xf8, 0x01, 0x00, 0xf8, 0x03, 0x01, 0xfc, 0x0f, 0x03, 0x7e, 0x00,
   0x07, 0xff, 0x00, 0x8e, 0x7f, 0x03, 0xfe, 0x7f, 0x00, 0xfc, 0x7f, 0x00,
   0xfc, 0x7f, 0x00, 0xf8, 0x3f, 0x00, 0xf0, 0x1f, 0x00, 0xe0, 0x1f, 0x00,
   0xc0, 0x0d, 0x00, 0xc0, 0x08, 0x00, 0x40, 0x08, 0x00, 0xc0, 0x18, 0x00 };

//Constructor
U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8

unsigned long dinoTimeToMove = 0;
unsigned long dinoTimeToDraw = 0;

int analogToDigitalIN;

int down = 0;
int jump = 0;
int dinoY = 43;
int cactiX = 130;

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2


void keyPress(void) {
  analogToDigitalIN = analogRead(0);    // read the value from the sensor 

  if (analogToDigitalIN < 1000 && dinoY == 44) {
    jump = 1;
    down = 1;
  }
}

void moveObjects() {
  if( millis() > dinoTimeToMove+50 ){
    moveDino();
    moveCactus();
    dinoTimeToMove = millis();  
  }
}

void moveDino(){
  if ( jump == 1 || dinoY <= 43 ) {
    jump = 0;
    if ( dinoY >= 20 && down == 1 )
      dinoY -= 1;
    else {
      down = 0;
      dinoY += 1;
    }
  }
}

void moveCactus(){
  cactiX -= 2;
  if ( cactiX <= -5 )
    cactiX = 130;
}

void draw(void) {
  if( millis() > dinoTimeToDraw + 25 ){
    do {
    drawDino( dinoY );
    drawSmallCactus( cactiX );
    drawScore();
    } while( u8g.nextPage() );
    dinoTimeToDraw = millis();
  }
}

void drawDino( int dinoY ){
  u8g.drawXBM(10, dinoY, dino_width, dino_height, dino_bits);
}

void drawSmallCactus( int cactiX ){
  u8g.drawXBM(cactiX, 47, smallcactus_width, smallcactus_height, smallcactus_bits);

}

void drawScore(void){
  char scoreBuff[50];
  String score = "Score: ";
  score += millis()/200;
  score.toCharArray(scoreBuff, 50);
  u8g.setFont(u8g_font_courB08);
  u8g.drawStr( 55, 8, scoreBuff );
}


void setup(void) {
  u8g.setRot180();
  u8g.setContrast( 3 );
  Serial.begin(9600);
}


void loop(void) {
  u8g.firstPage();  
  keyPress();
  moveObjects();
  draw();
}

