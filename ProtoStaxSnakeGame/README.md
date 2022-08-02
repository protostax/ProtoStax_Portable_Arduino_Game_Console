# ProtoStaxSnakeGame

An Arduino-based Snake Game with Background Music and Sound Effects in
a Portable Game Console!

![ProtoStaxSnakeGameDemo](ProtoStaxSnakeGameDemo.gif)

The objective of the game is to avoid obstacles and
eat apples. Every apple the snake eats makes its length increase by 1
and the game moves a little bit faster! The snake cannot run into
itself either (this becomes a danger when the snake gets longer!)

Press Button1 or Button2 to start the game. During gameplay, press
Button1 to make the snake turn counter-clockwise/left and Button2 to make
it turn clockwise/right. 
  
The game stops when the snake runs into an obstacle - either one of
the 4 edges/walls, or running into itself. Points are awarded based on
length of the snake. You have sound effects when the snake turns
clockwise, counter-clockwise, when it eats an apple, or when it hits
and obstacle and the game ends. There is some celebratory music when
you hit a high score!
  
Adapted from "Arduino 1602 Snake Game" by SadaleNet (https://github.com/SadaleNet/Arduino1602Snake)

This example uses the setup of the [ProtoStax Portable Arduino Game
Console](https://github.com/protostax/ProtoStax_Portable_Arduino_Game_Console) - please refer to that for parts, instructions and wiring.


## Prerequisites

This demo uses
* [ProtoStax for Arduino](https://www.protostax.com/products/protostax-for-arduino)
* [ProtoStax LCD Kit V2](https://www.protostax.com/products/protostax-lcd-kit-v2)
* [ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/products/protostax-kit-for-momentary-push-button-switches)
* Arduino (Uno)
* Piezo buzzers PS1240 x 2
* Jumper wires

![ProtoStax_Arduino_Game_Boy_bb](ProtoStax_Arduino_Game_Boy_bb.png)

For further instructions, see the [Make Your Own Arduino Game Boy
](https://www.hackster.io/sridhar-rajagopal/make-your-own-arduino-game-boy-7a6019) Tutorial at [Hackster.io](https://www.hackster.io/sridhar-rajagopal/make-your-own-arduino-game-boy-7a6019)

## Installing

This demo uses the following libraries:

* [MmlMusic](https://github.com/maxint-rd/MmlMusic)
* [ProtoStax_MmlMusicPlayer](https://github.com/protostax/ProtoStax_MmlMusicPlayer)
* LiquidCrystal_I2C
* JC_Button

```
Arduino IDE->Sketch->Include LIbrary->Manage Libraries - choose
JC_Button and install it
Arduino IDE->Sketch->Include LIbrary->Manage Libraries - choose
LiquidCrystal_I2C and install it
Install MmlMusic from GitHub (see link above)
Install ProtoStax_MmlMusicPlayer from GitHub (see link above)
```

Get the source code
```
git clone https://github.com/protostax/ProtoStax_Portable_Arduino_Game_Console.git
```

## Usage

See instructions in Tutorial linked above for putting together the circuit and
enclosure. 

```
open
ProtoStaxSnakeGame/ProtoStaxSnakeGame.ino in
Arduino IDE
compile sketch and upload to your Arduino
```

Enjoy!

## License

Written by Sridhar Rajagopal for ProtoStax. BSD license, all text above must be included in any redistribution

A lot of time and effort has gone into providing this and other code. Please support ProtoStax by purchasing products from us!

