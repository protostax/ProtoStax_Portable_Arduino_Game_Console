/*************************************************** 
  ProtoStax Endless Runner Game Example

  Press Button1 to start the game. During gameplay, press Button1 to jump
  and avoid obstacles. The game stops when the Runner (Harry Potter?!) runs into an obstacle (trees in a forest). 
  Points are awarded based on distance. The game tracks and displays the high score. 
  Pressing Button2 when the play is in progress loops through Background themes - there are 
  three themes - Harry Potter, Game of Thrones and Legend of Zelda. You also have sound effects
  when the Runner jumps, or when the Runner hits an obstacle and the game ends. 
  

  Adapted from "Arduino LCD Game" by Joshua Brooks (http://www.instructables.com/member/joshua.brooks/)
   
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

  In this example, music 1 plays the sound effects (jumping, hitting an obstacle etc), while music2 plays the background
  music theme.  

  The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for 
  additional information on the format.

  The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the MmlMusicPWM library, the bhagman Tone library,
  and the NewTone library. Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used). 

  It also uses the other following libraries:
  LiquidCrystal_I2C
  JC_Button
 
  Written by Sridhar Rajagopal for ProtoStax
  BSD license. All text above must be included in any redistribution
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ProtoStax_MmlMusicPlayer.h>
#include <JC_Button.h>

// We're using two pins for our Piezo speakers - 
// TONE_PIN_2 for background music and TONE_PIN_1 for sound effects
#define TONE_PIN_1 10
#define TONE_PIN_2 13

ProtoStax_MmlMusicPlayer music1;
ProtoStax_MmlMusicPlayer music2;

const char gameOfThrones[] PROGMEM = {"T80 O4 G8 C8 D#16 F16 G8 C8 D#16 F16 G8 C8 D#16 F16 G8 C8 D#16 F16 G8 C8 E16 F16 G8 C8 E16 F16 G8 C8 E16 F16 G8 C8 E16 F16  G.4 C.4 D#16 F16 G4 C4 D#16 F16 D.1 F.4 <A#.4> D16 D16 F4 <A#.4> D16 D16 C.1 G.4 C.4 D#16  F16  G4  C4  D#16  F16  D.1  F.4  <A#.4>   D#16  D16  F4  <A#.4>   D#16  D16  C.1   G.4  C.4   D#16  F16  G4   C4  D#16  F16   D.2   F.4  <A#.4>   D.8  D#.8  D.8  <A#.8>   C.1   >C.2<   A#.2   C.2  G.2 D#.2   D#.4  F.4    G.1   >C.2 <   A#.2   C.2   G.2    D#.2   D#.4  D.4 >C8<  G8  G#16  A#16  >C8<  G8  G#16  A#16 >C8<  G8  G#16  A#16  >C8<  G8  G#16  A#16 r4 >G#16 A#16 >C8<  G8   G#16 A#16 >C8<  G8  G#16  A#16  >C8<  G8  G#16  A#16< r r r r r r r r"};
const char harryPotter[] PROGMEM = {"T144 O6 r2 D4 G.4 A#8 A4 G2 >D4<  >C.2<   A.2  G.4 A#8 A4  F2 G#4  D.1   D4  G.4 A#8 A4  G2 >D4<  >F2< >E4<  >D#2< B4  >D#.4< >D8< >C#4<  C#2 B4  G.1  A#4  >D2< A#4  >D2< A#4  >D#2< >D4<  >C#2< A4  A#.4 >D8< >C#4<  C#2 D4  >D.1<   r4 A#4    >D2< A#4  >D2< A#4  >F2< >E4<  >D#2< B4  >D#.4< >D8< >C#4< C#2 A#4  G.1 r r r r r r r r"};
const char legendOfZelda[] PROGMEM = {"T88 O4  A#.2   F8   F8   A#8 G#16   F#16   G#.2   A#.2   F#8   F#8   A#8   A16   G16   A.2   r    A#4   F.4   A#8   A#16   >C16 D16 D#16  F2   F8   F8   F8   F#16  G#16   A#.2   A#8   A#8   G#8   F#16   G#.8   F#16   F2   F4    D#.8  F16  F#2  F8  D#8    C#.8  D#16  F2  D#8  C#8   C#.8  D16  E2  G8    F16<  F16  F16  F16 F16 F16 F16 F16 F8  F16 F8   A#4   F.4   A#8   A#16   >C16  D16  D#16   F2   F8   F8   F8   F#16  G#16   A#.2  >C#4   C4<  A2  F4   F#.2  A#4   A4  F2  F4   F#.2  A#4   A4  F2  D4   D#.2  F#4   F4  C#2<  A#4   >C.8  D16  E2  G8    F16<  F16  F16  F16 F16 F16 F16 F16 F8  F16 F8 r r r r r r r r"};

// Used for the start, and when jumping
const char blurb[] PROGMEM = {"T240 L16 O5 C D E F G"};

// Used when the game ends due to running into obstacle
const char uhoh[] PROGMEM = {"T255 L32 O7 CC+DD+EFF+GG+AA+BA+AG+GF+FED+DC+C"};

// Used to control the Runner jumping
#define PIN_BUTTON1 2

// Used to toggle between theme music
#define PIN_BUTTON2 3

// Set the auto pin to PIN_BUTTON_1 to autoplay! 
#define PIN_AUTOPLAY 1

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.'         // Use the '.' character for the head
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '      // User the ' ' character
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7

#define HERO_HORIZONTAL_POSITION 1    // Horizontal position of hero on screen

#define TERRAIN_WIDTH 16
#define TERRAIN_EMPTY 0
#define TERRAIN_LOWER_BLOCK 1
#define TERRAIN_UPPER_BLOCK 2

#define HERO_POSITION_OFF 0          // Hero is invisible
#define HERO_POSITION_RUN_LOWER_1 1  // Hero is running on lower row (pose 1)
#define HERO_POSITION_RUN_LOWER_2 2  //                              (pose 2)

#define HERO_POSITION_JUMP_1 3       // Starting a jump
#define HERO_POSITION_JUMP_2 4       // Half-way up
#define HERO_POSITION_JUMP_3 5       // Jump is on upper row
#define HERO_POSITION_JUMP_4 6       // Jump is on upper row
#define HERO_POSITION_JUMP_5 7       // Jump is on upper row
#define HERO_POSITION_JUMP_6 8       // Jump is on upper row
#define HERO_POSITION_JUMP_7 9       // Half-way down
#define HERO_POSITION_JUMP_8 10      // About to land

#define HERO_POSITION_RUN_UPPER_1 11 // Hero is running on upper row (pose 1)
#define HERO_POSITION_RUN_UPPER_2 12 //                              (pose 2)

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

static char terrainUpper[TERRAIN_WIDTH + 1];
static char terrainLower[TERRAIN_WIDTH + 1];
static bool buttonPushed = false;
static bool button2Pushed = false;

Button button2(PIN_BUTTON2);       // define the button

static int musicIndex = 0;

void initializeGraphics(){
  static byte graphics[] = {
    // Run position 1 - Harry Potter With Wand!
    B01100,
    B01100,
    B00001,
    B01110,
    B11100,
    B01100,
    B11010,
    B10011,
    // Run position 2 - Harry Potter With Wand!
    B01100,
    B01100,
    B00001,
    B01110,
    B01100,
    B01100,
    B01100,
    B01110,
    // Jump
    B01100,
    B01100,
    B00000,
    B11110,
    B01101,
    B11111,
    B10000,
    B00000,
    // Jump lower - Harry Potter With Wand!
    B11110,
    B01101,
    B11111,
    B10000,
    B00000,
    B00000,
    B00000,
    B00000,
    // Ground - Tree
    B00100,
    B01110,
    B11111,
    B00100,
    B01110,
    B11111,
    B00100,
    B00100,
    // Ground right - Tree
    B00001,
    B00011,
    B00111,
    B00001,
    B00011,
    B00111,
    B00001,
    B00001,
    // Ground left - Tree       
    B10000,
    B11000,
    B11100,
    B10000,
    B11000,
    B11100,
    B10000,
    B10000,
  };
  int i;
  // Skip using character 0, this allows lcd.print() to be used to
  // quickly draw multiple characters
  for (i = 0; i < 7; ++i) {
    lcd.createChar(i + 1, &graphics[i * 8]);
  }
  for (i = 0; i < TERRAIN_WIDTH; ++i) {
    terrainUpper[i] = SPRITE_TERRAIN_EMPTY;
    terrainLower[i] = SPRITE_TERRAIN_EMPTY;
  }
}

// Slide the terrain to the left in half-character increments
//
void advanceTerrain(char* terrain, byte newTerrain){
  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    char current = terrain[i];
    char next = (i == TERRAIN_WIDTH-1) ? newTerrain : terrain[i+1];
    switch (current){
      case SPRITE_TERRAIN_EMPTY:
        terrain[i] = (next == SPRITE_TERRAIN_SOLID) ? SPRITE_TERRAIN_SOLID_RIGHT : SPRITE_TERRAIN_EMPTY;
        break;
      case SPRITE_TERRAIN_SOLID:
        terrain[i] = (next == SPRITE_TERRAIN_EMPTY) ? SPRITE_TERRAIN_SOLID_LEFT : SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_RIGHT:
        terrain[i] = SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_LEFT:
        terrain[i] = SPRITE_TERRAIN_EMPTY;
        break;
    }
  }
}

bool drawHero(byte position, char* terrainUpper, char* terrainLower, unsigned int score) {
  bool collide = false;
  char upperSave = terrainUpper[HERO_HORIZONTAL_POSITION];
  char lowerSave = terrainLower[HERO_HORIZONTAL_POSITION];
  byte upper, lower;
  switch (position) {
    case HERO_POSITION_OFF:
      upper = lower = SPRITE_TERRAIN_EMPTY;
      break;
    case HERO_POSITION_RUN_LOWER_1:
      upper = SPRITE_TERRAIN_EMPTY;
      lower = SPRITE_RUN1;
      break;
    case HERO_POSITION_RUN_LOWER_2:
      upper = SPRITE_TERRAIN_EMPTY;
      lower = SPRITE_RUN2;
      break;
    case HERO_POSITION_JUMP_1:
    case HERO_POSITION_JUMP_8:
      upper = SPRITE_TERRAIN_EMPTY;
      lower = SPRITE_JUMP;
      break;
    case HERO_POSITION_JUMP_2:
    case HERO_POSITION_JUMP_7:
      upper = SPRITE_JUMP_UPPER;
      lower = SPRITE_JUMP_LOWER;
      break;
    case HERO_POSITION_JUMP_3:
    case HERO_POSITION_JUMP_4:
    case HERO_POSITION_JUMP_5:
    case HERO_POSITION_JUMP_6:
      upper = SPRITE_JUMP;
      lower = SPRITE_TERRAIN_EMPTY;
      break;
    case HERO_POSITION_RUN_UPPER_1:
      upper = SPRITE_RUN1;
      lower = SPRITE_TERRAIN_EMPTY;
      break;
    case HERO_POSITION_RUN_UPPER_2:
      upper = SPRITE_RUN2;
      lower = SPRITE_TERRAIN_EMPTY;
      break;
  }
  if (upper != ' ') {
    terrainUpper[HERO_HORIZONTAL_POSITION] = upper;
    collide = (upperSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  if (lower != ' ') {
    terrainLower[HERO_HORIZONTAL_POSITION] = lower;
    collide |= (lowerSave == SPRITE_TERRAIN_EMPTY) ? false : true;
  }
  
  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;
  
  // Draw the scene
  terrainUpper[TERRAIN_WIDTH] = '\0';
  terrainLower[TERRAIN_WIDTH] = '\0';
  char temp = terrainUpper[16-digits];
  terrainUpper[16-digits] = '\0';
  lcd.setCursor(0,0);
  lcd.print(terrainUpper);
  terrainUpper[16-digits] = temp;  
  lcd.setCursor(0,1);
  lcd.print(terrainLower);
  
  lcd.setCursor(16 - digits,0);
  lcd.print(score);

  terrainUpper[HERO_HORIZONTAL_POSITION] = upperSave;
  terrainLower[HERO_HORIZONTAL_POSITION] = lowerSave;
  return collide;
}

// Handle the button push as an interrupt
// We handle Button1 press via an interrupt to handle jump as soon as possible
// Debouncing does not matter as much in this case and faster response is desirable
void buttonPush() {
  buttonPushed = true;
}

// We're using JC_Button to handle Button2 press, to debounce
// and make the press more deterministic

// Handle the button2 push as an interrupt
//void button2Push() {
//  button2Pushed = true;
//}


void setup(){
  Serial.begin(9600);
  
  pinMode(PIN_BUTTON1, INPUT);
  digitalWrite(PIN_BUTTON1, HIGH);
  // Button2 is initialized as JC_Button
  // so the following is not necessary
  //pinMode(PIN_BUTTON2, INPUT);
  //digitalWrite(PIN_BUTTON2, HIGH);
  button2.begin();
  
  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, HIGH);
  
  // Digital pin 2 maps to interrupt 0
  attachInterrupt(0/*PIN_BUTTON1*/, buttonPush, FALLING);

  // Using JC_Button to debounce, so not using interrupts for this button
  // Digital pin 3 maps to interrupt 1
  // attachInterrupt(1/*PIN_BUTTON2*/, button2Push, FALLING);  
  
  initializeGraphics();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Runner");
  music1.begin(TONE_PIN_1);
  delay(100);
  music2.begin(TONE_PIN_2);
  
  music1.play_P(blurb);

}

void loop(){
  static int melodyNum = 1;
  static int num = 0;
  static int startMusic = 0;
  
  static byte heroPos = HERO_POSITION_RUN_LOWER_1;
  static byte newTerrainType = TERRAIN_EMPTY;
  static byte newTerrainDuration = 1;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;
  static unsigned int highDistance = 0;

  button2.read();

  
  // music1.continuePlaying(); 
  if (!playing) {
    drawHero((blink) ? HERO_POSITION_OFF : heroPos, terrainUpper, terrainLower, distance >> 3);
    if (blink) {
      lcd.setCursor(0,0);
      lcd.print("Press Start");
      // Display the high score
      unsigned int hs = highDistance >> 3;
      byte digits = (hs > 9999) ? 5 : (hs > 999) ? 4 : (hs > 99) ? 3 : (hs > 9) ? 2 : 1;
      lcd.setCursor(16-digits,1);
      lcd.print(hs);
    }
    delay(250);
    blink = !blink;
    if (buttonPushed) {
      initializeGraphics();
      heroPos = HERO_POSITION_RUN_LOWER_1;
      playing = true;
      buttonPushed = false;
      distance = 0;
    }
    return;
  }

  if (!music2.isPlaying()) {
      music2.play_P(harryPotter);
  } 
    
    {
      // Shift the terrain to the left
      advanceTerrain(terrainLower, newTerrainType == TERRAIN_LOWER_BLOCK ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);
      advanceTerrain(terrainUpper, newTerrainType == TERRAIN_UPPER_BLOCK ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);
      
      // Make new terrain to enter on the right
      if (--newTerrainDuration == 0) {
        if (newTerrainType == TERRAIN_EMPTY) {
          newTerrainType = (random(3) == 0) ? TERRAIN_UPPER_BLOCK : TERRAIN_LOWER_BLOCK;
          newTerrainDuration = 2 + random(10);
        } else {
          newTerrainType = TERRAIN_EMPTY;
          newTerrainDuration = 10 + random(10);
        }
      }
        
      if (buttonPushed) {
        if (heroPos <= HERO_POSITION_RUN_LOWER_2) { 
          heroPos = HERO_POSITION_JUMP_1;
          music1.play_P(blurb); // give a short blurp              
        }
        buttonPushed = false;
      }  

      if (button2.wasReleased()) {
        musicIndex = (musicIndex+1) % 3;
        music2.stop();
        if (!music2.isPlaying()) {
          switch (musicIndex) {
            case 0: 
              music2.play_P(harryPotter);
              break;
            case 1:
              music2.play_P(gameOfThrones);
              break;
            case 2:
              music2.play_P(legendOfZelda);
              break;
          }
        }
        // Not necessary since we're using JC_Button to handle button press
        //button2Pushed = false;
      }
    
      if (drawHero(heroPos, terrainUpper, terrainLower, distance >> 3)) {
        playing = false; // The hero collided with something. Too bad.
      } else {
        if (heroPos == HERO_POSITION_RUN_LOWER_2 || heroPos == HERO_POSITION_JUMP_8) {
          heroPos = HERO_POSITION_RUN_LOWER_1;
        } else if ((heroPos >= HERO_POSITION_JUMP_3 && heroPos <= HERO_POSITION_JUMP_5) && terrainLower[HERO_HORIZONTAL_POSITION] != SPRITE_TERRAIN_EMPTY) {
          heroPos = HERO_POSITION_RUN_UPPER_1;
        } else if (heroPos >= HERO_POSITION_RUN_UPPER_1 && terrainLower[HERO_HORIZONTAL_POSITION] == SPRITE_TERRAIN_EMPTY) {
          heroPos = HERO_POSITION_JUMP_5;
        } else if (heroPos == HERO_POSITION_RUN_UPPER_2) {
          heroPos = HERO_POSITION_RUN_UPPER_1;
        } else {
          ++heroPos;
        }
        ++distance;
        
        digitalWrite(PIN_AUTOPLAY, terrainLower[HERO_HORIZONTAL_POSITION + 2] == SPRITE_TERRAIN_EMPTY ? HIGH : LOW);
      }
      delay(100);
      
    }
    if (!playing) {
      music1.play_P(uhoh);    
      music2.stop();    
      if (distance > highDistance) {
        // Keep track of high score!
        highDistance = distance;
      }
      return;
    }    

}
