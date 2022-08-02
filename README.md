# ProtoStax_Portable_Arduino_Game_Console

A Portable Game Console built with:
* [ProtoStax for Arduino](https://www.protostax.com/products/protostax-for-arduino)
* [ProtoStax LCD Kit V2](https://www.protostax.com/products/protostax-lcd-kit-v2)
* [ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/products/protostax-kit-for-momentary-push-button-switches)
* Arduino Uno
* Piezo buzzers PS1240 x 2
* Jumper wires

![ProtoStax_Portable_Arduino_Game_Console_Demo](ProtoStax_Portable_Arduino_Game_Console_Demo.gif)

This console gives you a portable game development environment using a 16x2 Char LCD, two buttons for control, and 2 piezo speakers for background music and sound effects, all in a portable ProtoStax Enclosure system. 

Of course, what good is a portable game console if it doesn't have
some demo games? :-) We have a couple to get you started, and will add
more along the way. If you have developed other games using this,
please feel free to contribute them back here via a git pull request!

## Prerequisites

This Portable Arduino Game Console uses:
* [ProtoStax for Arduino](https://www.protostax.com/products/protostax-for-arduino)
* [ProtoStax LCD Kit V2](https://www.protostax.com/products/protostax-lcd-kit-v2)
* [ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/products/protostax-for-arduino)
* Arduino (Uno)
* Piezo buzzers PS1240 x 2
* Jumper wires

![ProtoStax_Arduino_Game_Boy_bb](ProtoStax_Arduino_Game_Boy_bb.png)

For further instructions, see the [Portable Arduino Game Console - Part 1 - Endless Runner](https://www.hackster.io/sridhar-rajagopal/portable-arduino-game-console-part-1-endless-runner-7a6019)
Tutorial at
[Hackster.io](https://www.hackster.io/sridhar-rajagopal/portable-arduino-game-console-part-1-endless-runner-7a6019)

Also see
[Portable Arduino Game Console - Part 2 - Snake Game](https://www.hackster.io/sridhar-rajagopal/portable-arduino-game-console-part-2-snake-game-ab5137)

## Games

### Endless Runner Game
Guide the Runner (Harry Potter (with his wand)?!), jumping over and avoiding the obstacles (Trees in the Forbidden Forest?!).

### Snake Game
The objective of the game is to avoid obstacles and eat apples. Every apple the snake eats makes its length increase by 1
and the game moves a little bit faster! The snake cannot run into itself either (this becomes a danger when the snake gets longer!)

## Installing

The demos use the following libraries:

* [MmlMusic](https://github.com/maxint-rd/MmlMusic)
* [ProtoStax_MmlMusicPlayer](https://github.com/protostax/ProtoStax_MmlMusicPlayer)
* LiquidCrystal_I2C
* JC_Button (*some demos use this)

```
Arduino IDE->Sketch->Include LIbrary->Manage Libraries - choose
JC_Button and install it
Arduino IDE->Sketch->Include LIbrary->Manage Libraries - choose
LiquidCrystal_I2C and install it
Install MmlMusic from GitHub
Install ProtoStax_MmlMusicPlayer from GitHub
```

Get the source code
```
git clone https://github.com/protostax/ProtoStax_Portable_Arduino_Game_Console.git
```

## Usage

See instructions in Tutorial linked above for putting together the circuit and
enclosure. 

For further instructions, see the README under each Demo Game's directory

## License

Written by Sridhar Rajagopal for ProtoStax. BSD license, all text above must be included in any redistribution

A lot of time and effort has gone into providing this and other code. Please support ProtoStax by purchasing products from us!

