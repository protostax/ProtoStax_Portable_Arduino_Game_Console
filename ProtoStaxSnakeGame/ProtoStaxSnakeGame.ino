/*************************************************** 
  ProtoStax Snake Game Example

  The objective of the game is to avoid obstacles and eat apples. Every 
  apple the snake eats makes its length increase by 1 and the game moves a little bit faster! The snake cannot run
  into itself either (this becomes a danger when the snake gets longer!). 

  Press Button1 or Button2 to start the game. During gameplay, press Button1 to make the snake turn 
  counter-clockwise/left and Button2 to make it turn clockwise/right.
  
  The game stops when the snake runs into an obstacle - either one of the 4 edges/walls, or running into itself. 
  Points are awarded based on length of the snake. You have sound effects
  when the snake turns clockwise/right, counter-clockwise/left, when it eats an apple, or when it hits and obstacle 
  and the game ends. There is some celebratory music when you hit a high score!

  Adapted from "Arduino 1602 Snake Game" by SadaleNet (https://github.com/SadaleNet/Arduino1602Snake)
  
  Sorry SadaleNet, we had initially attributed to it to another source that had taken your code and not 
  given you attribution (but had interesting left the lcd.print("- Sadale.net") , though commented out, which is 
  how we found you eventually! Well written game! Bravo! Hope you like our upgrades! :-). 
  We also had to figure out the naunces of the code by ourselves - too bad we hadn't seen 
  your well-written writeup then! 
   
  It uses
  Arduino Uno,
  ProtoStax for Arduino --> https://www.protostax.com/products/protostax-for-arduino
  ProtoStax LCD Kit V2 --> https://www.protostax.com/products/protostax-lcd-kit-v2,
  ProtoStax Kit for Momentary Push Button Switches --> 
     https://www.protostax.com/products/protostax-kit-for-momentary-push-button-switches

  Piezo Buzzer PS1240 x2,
  Jumper wires 

  The following pins are used in this example: 
  SCL and SDA pins for I2C communications with the 16x2 LCD I2C module, pins 10 and 13 for the 
  piezo buzzers, and 
  pins 2 and 3 for the push buttons. 

  This uses the ProtoStax_MmlMusicPlayer library, which extends the MmlMusic library to provide autoplay 
  capabilities using the Timers on the Arduino. Muliple ProtoStax_MmlMusicPlayer objects can be 
  instantiated, and each will use an available timer. On the Arduino Uno, there are two Timers - Timer1 and Timer2 -
  available, so two objects can be instantiated. You can have simultaneous autoplay of two different music tracks. 

  In this example, music 1 plays the sound effects (jumping, hitting an obstacle etc), while music2 plays the apple eating music.  

  The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for 
  additional information on the format.

  The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the MmlMusicPWM library, the bhagman Tone library,
  and the NewTone library. Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used). 

  It also uses the other following libraries:
  LiquidCrystal_I2C
 
  Written by Sridhar Rajagopal for ProtoStax
  BSD license. All text above must be included in any redistribution
 */

/*
 * Copyright message of the original code
 * 
Copyright 2018 Wong Cho Ching <sadale.net>
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions 
are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

                  
#include <ProtoStax_MmlMusicPlayer.h>
#include <LiquidCrystal_I2C.h> 
#include <stdlib.h>
#include <limits.h>


// We're using two pins for our Piezo speakers - 
// TONE_PIN_2 for background music and TONE_PIN_1 for sound effects
#define TONE_PIN_1 10
#define TONE_PIN_2 13

ProtoStax_MmlMusicPlayer music1;
ProtoStax_MmlMusicPlayer music2;

// A little intro music based on Pac-Man!
// XXX - TODO: dotted sharp notes f#.16 don't seem to be working that well with MmlMusic
// As a fix, modified it to F#16 F#32 though it's not technically the same, it is close enough!
const char introMusic[] PROGMEM = {"T105 O5 b16 >b16 f#16 d#16 b32 f#16 f#32 d#8 c16 >c16 <g16 e16 >c32 <g.16 e8 <b16 >b16 f#16 d#16 b32 f#16 f#32 d#8 d#32 e32 f16 f32 f#32 g16 g32 g#32 a16 b16"};
// Add a little chords to the intro music using our music2 player! :-) 
const char introMusic2[] PROGMEM = {"T105 o4 b.8 >b16 <b.8. >b16 c.8 >c16 <c.8. >c16 <<b.8 >b16 <b.8 >b16 f#8 g#8 a#8 b16"};


const char moveLeftMusic[] PROGMEM = {"T240 L16 O5 C D E F G"};
const char moveRightMusic[] PROGMEM = {"T240 L16 O5 G F E D C"};

const char appleEaten[] PROGMEM = {"T120 L16 O5 C D E G. R E G. R"};

// Used when the game ends due to running into obstacle
const char uhoh[] PROGMEM = {"T255 L32 O7 CC+DD+EFF+GG+AA+BA+AG+GF+FED+DC+C"};

// An homage to Mappy?! 
const char highScoreMusic[] PROGMEM = {"T105 L16 O5 <B> C G <B> C G C G  <B#> C# G <B> C G C G R8 B- B A. D G. R8 G."};


enum DisplayItem {GRAPHIC_ITEM_NONE, GRAPHIC_ITEM_A, GRAPHIC_ITEM_B,
                  GRAPHIC_ITEM_NUM};

#define GRAPHIC_WIDTH 16
#define GRAPHIC_HEIGHT 4
LiquidCrystal_I2C lcd(0x27,16,2);

byte block[3] = {
  B01110,
  B01110,
  B01110,
};

byte apple[3] = {
  B00100,
  B01010,
  B00100,
};
#define DEBOUNCE_DURATION 20
//Return true if the actual output value is true
bool debounce_activate(unsigned long* debounceStart)
{
  if(*debounceStart == 0)
    *debounceStart = millis();
  else if(millis()-*debounceStart>DEBOUNCE_DURATION)
    return true;
  return false;
}
//Return true if it's rising edge/falling edge
bool debounce_activate_edge(unsigned long* debounceStart)
{

  if(*debounceStart == ULONG_MAX){
    return false;
  }else if(*debounceStart == 0){
    *debounceStart = millis();
  }else if(millis()-*debounceStart>DEBOUNCE_DURATION){
    *debounceStart = ULONG_MAX;
    return true;
  }
  return false;
}
void debounce_deactivate(unsigned long* debounceStart){
  *debounceStart = 0;
}
#define BUTTON_LEFT 2 //LEFT BUTTON
#define BUTTON_RIGHT 3 // RIGHT BUTTON
unsigned long debounceCounterButtonLeft = 0;
unsigned long debounceCounterButtonRight = 0;
struct Pos {
  uint8_t x=0, y=0;
};
struct Pos snakePosHistory[GRAPHIC_HEIGHT*GRAPHIC_WIDTH]; 
//first element is the head.
size_t snakeLength = 0;
size_t highScore = 0;

enum {SNAKE_LEFT,SNAKE_UP,SNAKE_RIGHT,SNAKE_DOWN} snakeDirection;
struct Pos applePos;
unsigned long lastGameUpdateTick = 0;
unsigned long gameUpdateInterval = 800;
bool thisFrameControlUpdated = false;
enum {GAME_MENU, GAME_PLAY, GAME_LOSE, GAME_WIN} gameState;
enum {GAME_LOSE_WALL_COLLISION, GAME_LOSE_SELF_COLLISION} gameLoseReason;

uint8_t graphicRam[GRAPHIC_WIDTH*2/8][GRAPHIC_HEIGHT];
void graphic_generate_characters()
{
  /*
    space: 0 0
    
    0: 0 A
    1: 0 B
    2: A 0
    3: A A
    4: A B
    5: B 0
    6: B A
    7: B B

    Each glyph occupies only half an 8x5 character block. These represent the 8 combinations of 
    snake, apple or empty space in each half. For example, you can have snake and snake in the 
    top and bottom halfs because the snake can go around in circles. There can only be 8 
    custom characters created, so this puts a limit on the cute customizations you can 
    create for the glyphys, unfortunately.
  */
  for (size_t i=0; i<8; i++) {
    byte glyph[8];
    int upperIcon = (i+1)/3;
    int lowerIcon = (i+1)%3;
    memset(glyph, 0, sizeof(glyph));
    if(upperIcon==1)
      memcpy(&glyph[0], &block[0], 3);
    else if(upperIcon==2)
      memcpy(&glyph[0], &apple[0], 3);
    if(lowerIcon==1)
      memcpy(&glyph[4], &block[0], 3);
    else if(lowerIcon==2)
      memcpy(&glyph[4], &apple[0], 3);
    
    lcd.createChar(i, glyph);
  }
  delay(1); 
  //Wait for the CGRAM to be written
}
void graphic_clear(){
  memset(graphicRam, 0, sizeof(graphicRam));
}
void graphic_add_item(uint8_t x, uint8_t y, enum DisplayItem item) {
  graphicRam[x/(8/2)][y] |= (uint8_t)item*(1<<((x%(8/2))*2));
}
void graphic_flush(){
  lcd.clear();
  for(size_t x=0; x<16; x++){
    for(size_t y=0; y<2; y++){
      enum DisplayItem upperItem =
        (DisplayItem)((graphicRam[x/(8/2)][y*2]>>((x%(8/2))*2))&0x3);
      enum DisplayItem lowerItem =
        (DisplayItem)((graphicRam[x/(8/2)][y*2+1]>>((x%(8/2))*2))&0x3);
      if(upperItem>=GRAPHIC_ITEM_NUM)
        upperItem = GRAPHIC_ITEM_B;

      if(lowerItem>=GRAPHIC_ITEM_NUM)
        lowerItem = GRAPHIC_ITEM_B;
      lcd.setCursor(x, y);
      if(upperItem==0 && lowerItem==0)
        lcd.write(' ');
      else
        lcd.write(byte((uint8_t)upperItem*3+(uint8_t)lowerItem-1));
    }
  }
}
void game_new_apple_pos()
{
  bool validApple = true;
  do{
    applePos.x = rand()%GRAPHIC_WIDTH;
    applePos.y = rand()%GRAPHIC_HEIGHT;
    validApple = true;
    for(size_t i=0; i<snakeLength; i++)
      {
        if(applePos.x==snakePosHistory[i].x && applePos.y==snakePosHistory[i].y){
          validApple = false;
          break;
        }
      }
  } while(!validApple);
}
void game_init(){

  
  srand(micros());
  gameUpdateInterval = 1000;
  gameState = GAME_PLAY;
  snakePosHistory[0].x=3; snakePosHistory[0].y=1;
  snakePosHistory[1].x=2; snakePosHistory[1].y=1;
  snakePosHistory[2].x=1; snakePosHistory[2].y=1;
  snakePosHistory[3].x=0; snakePosHistory[3].y=1;
  snakeLength = 4;
  snakeDirection = SNAKE_RIGHT;
  game_new_apple_pos();
  thisFrameControlUpdated = false;
}
void game_calculate_logic() {

  
  if(gameState!=GAME_PLAY)
    return;
  //Calculate the movement of the tail
  for(size_t i=snakeLength; i>=1; i--){ 
    snakePosHistory[i] = snakePosHistory[i-1];
  }
  //Calculate the head movement
  snakePosHistory[0]=snakePosHistory[1];
  switch(snakeDirection){
  case SNAKE_LEFT: snakePosHistory[0].x--; break;
  case SNAKE_UP: snakePosHistory[0].y--; break;
  case SNAKE_RIGHT: snakePosHistory[0].x++; break;
  case SNAKE_DOWN: snakePosHistory[0].y++; break;
  }
  //Look for wall collision
  if(snakePosHistory[0].x<0||snakePosHistory[0].x>=GRAPHIC_WIDTH||snakePosHistory[0].y
     <0||snakePosHistory[0].y>=GRAPHIC_HEIGHT){
    gameState = GAME_LOSE;
    gameLoseReason = GAME_LOSE_WALL_COLLISION;
    return;
  }
  //Look for self collision
  for(size_t i=1; i<snakeLength; i++){
    if(snakePosHistory[0].x==snakePosHistory[i].x &&
       snakePosHistory[0].y==snakePosHistory[i].y){
      gameState = GAME_LOSE;
      gameLoseReason = GAME_LOSE_SELF_COLLISION;     
      return;
    }
  }
  if(snakePosHistory[0].x==applePos.x && snakePosHistory[0].y==applePos.y){
    music2.play_P(appleEaten);

    snakeLength++;
    gameUpdateInterval = gameUpdateInterval*9/10;
    if(snakeLength>=sizeof(snakePosHistory)/sizeof(*snakePosHistory))
      gameState = GAME_WIN;
    else
      game_new_apple_pos();
  }
}
void game_calculate_display() {

  
  graphic_clear();
  switch(gameState){
  case GAME_LOSE:
    music1.play_P(uhoh);
    while (music1.isPlaying());    
    lcd.clear();
    lcd.setCursor(0, 0);
    if (gameLoseReason == GAME_LOSE_WALL_COLLISION) {
      lcd.print("You hit a wall!");
    } else if (gameLoseReason == GAME_LOSE_SELF_COLLISION) {
      lcd.print("You ate yourself");      
    }
    lcd.setCursor(0, 1);
    lcd.print("Length: ");
    lcd.setCursor(8, 1);
    lcd.print(snakeLength);
    lcd.print("(*");
    lcd.print(highScore);  
    lcd.print(")");
    // Let's use a small delay to keep the messaging
    delay(2000);    
    if (snakeLength > highScore) {
        // Keep track of high score!
        highScore = snakeLength;
        lcd.setCursor(0, 0);
        lcd.print("Congratulations!");
        lcd.setCursor(0, 1);
        lcd.print("  High score!   ");      
        music1.play_P(highScoreMusic);
     }
    while (music1.isPlaying());
   
    gameState = GAME_MENU;
    break;
  case GAME_WIN:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You won. Congratz!");
    lcd.setCursor(0, 1);
    lcd.print("Length: ");
    lcd.setCursor(8, 1);
    lcd.print(snakeLength);
    break;
  case GAME_PLAY:
    for(size_t i=0; i<snakeLength; i++)
      graphic_add_item(snakePosHistory[i].x, snakePosHistory[i].y, GRAPHIC_ITEM_A);
    graphic_add_item(applePos.x, applePos.y, GRAPHIC_ITEM_B);
    graphic_flush();
    break;
  case GAME_MENU:
    lcd.setCursor(0,0);
    lcd.print("Press Button to ");
    lcd.setCursor(0,1);
    lcd.print("     Start      ");
    break;
  }
}
void setup() {

  Serial.begin(9600);

  
  lcd.init();
  lcd.backlight();
  lcd.print("   ProtoStax   ");
  lcd.setCursor(0, 1);
  lcd.print("   Snake Game   ");
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);  
  
  graphic_generate_characters();
  gameState = GAME_MENU;

  music1.begin(TONE_PIN_1);
  delay(100);
  music2.begin(TONE_PIN_2);
  
  music1.play_P(introMusic);
  music2.play_P(introMusic2);

  // Let's wait until introMusic finishes playing! 
  while(music1.isPlaying());
  
}
void loop() {

  
  lcd.setCursor(0, 0);
  if(digitalRead(BUTTON_LEFT)==LOW){
    if(debounce_activate_edge(&debounceCounterButtonLeft)&&!thisFrameControlUpdated){
      music1.play_P(moveLeftMusic);
      
      switch(gameState){
      case GAME_PLAY:
        switch(snakeDirection){
        case SNAKE_LEFT: snakeDirection=SNAKE_DOWN; break;
        case SNAKE_UP: snakeDirection=SNAKE_LEFT; break;
        case SNAKE_RIGHT: snakeDirection=SNAKE_UP; break;
        case SNAKE_DOWN: snakeDirection=SNAKE_RIGHT; break;
        }
        thisFrameControlUpdated = true;
        break;
      case GAME_MENU:
        game_init();
        break;
      case GAME_LOSE:
        gameState = GAME_MENU;
        break;
      }
    }
  }else{
    debounce_deactivate(&debounceCounterButtonLeft);
  }
  lcd.setCursor(8, 0);
  if(digitalRead(BUTTON_RIGHT)==LOW){
    if(debounce_activate_edge(&debounceCounterButtonRight)&&!thisFrameControlUpdated){
      music1.play_P(moveRightMusic);
      
      switch(gameState){
      case GAME_PLAY:
        switch(snakeDirection){
        case SNAKE_LEFT: snakeDirection=SNAKE_UP; break;
        case SNAKE_UP: snakeDirection=SNAKE_RIGHT; break;
        case SNAKE_RIGHT: snakeDirection=SNAKE_DOWN; break;
        case SNAKE_DOWN: snakeDirection=SNAKE_LEFT; break;
        }
        thisFrameControlUpdated = true;
        break;
      case GAME_MENU:
        game_init();
        break;
      }
    }
  }else{
    debounce_deactivate(&debounceCounterButtonRight);
  }
  if(millis()-lastGameUpdateTick>gameUpdateInterval){
    game_calculate_logic();
    game_calculate_display();
    lastGameUpdateTick = millis();
    thisFrameControlUpdated = false;
  }
}
