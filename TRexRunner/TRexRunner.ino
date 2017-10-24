/*

  Bitmap.pde
  
  Show simple bitmap

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/


#include "U8glib.h"
#define dino_width 21
#define dino_height 20

static unsigned char dino_bits[] = {
   0x00, 0xf0, 0x0f, 0x00, 0xd8, 0x1f, 0x00, 0xf8, 0x1f, 0x00, 0xf8, 0x1f,
   0x00, 0xf8, 0x01, 0x00, 0xf8, 0x03, 0x01, 0xfc, 0x0f, 0x03, 0x7e, 0x00,
   0x07, 0xff, 0x00, 0x8e, 0x7f, 0x03, 0xfe, 0x7f, 0x00, 0xfc, 0x7f, 0x00,
   0xfc, 0x7f, 0x00, 0xf8, 0x3f, 0x00, 0xf0, 0x1f, 0x00, 0xe0, 0x1f, 0x00,
   0xc0, 0x0d, 0x00, 0xc0, 0x08, 0x00, 0x40, 0x08, 0x00, 0xc0, 0x18, 0x00 };

//Constructor
U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);  // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8


int analogToDigitalIN;
int key=-1;
int oldkey=-1;

int down = 0;
int jump = 0;
int dinoY = 43;

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2

int getKey(unsigned int input) {   
    if (input < 100) return 0;
    else  if (input < 300) return 1;
    else  if (input < 500) return 2;
    else  if (input < 700) return 3;
    else  if (input < 900) return 4;    
    else  return -1;
}


void keyPress(void) {
  analogToDigitalIN = analogRead(0);    // read the value from the sensor  
  key = getKey(analogToDigitalIN);   // convert into key press  
  if (key != oldkey) {   // if keypress is detected
    delay(50);    // wait for debounce time
    analogToDigitalIN = analogRead(0);    // read the value from the sensor  
    key = getKey(analogToDigitalIN);     // convert into key press
    if (key != oldkey) {     
      oldkey = key;
      if (key >=0){
         if ( key == 0 )    {
                    
         }
         else if ( key == 1 ){
          
         }
         else if ( key == 2 ){
          
         }
         else if ( key == 3 ){
          
         }
         else if ( key == 4 && dinoY == 44 ){
          jump = 1;
          down = 1;
          key = 0;
         } else {
          
         }
      }
    }
  }
 delay(100);
}


void moveDino(){
  if ( jump == 1 || dinoY <= 43 ) {
    jump = 0;
    if ( dinoY >= 28 && down == 1 )
      dinoY -= 1;
    else {
      down = 0;
      dinoY += 1;
    }
  }
}

void draw(void) {
  drawDino( dinoY );
  drawScore();
}

void drawDino( int dinoY ){
  u8g.drawXBM(1, dinoY, dino_width, dino_height, dino_bits);
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
  Serial.begin(9600);
}


void loop(void) {
  
  u8g.firstPage();  
  do {
    keyPress();
    moveDino();
    draw();
  } while( u8g.nextPage() );

  delay(100);
}

