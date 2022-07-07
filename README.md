# ProtoStax_Endless_Runner_Game

Demo for ProtoStax Arduino Game Boy with Arduino

![ProtoStax_Endless_Runner_Game_Demo](ProtoStax_Endless_Runner_Game_Demo.gif)

using
[ProtoStax for Arduino](https://www.protostax.com/products/protostax-for-arduino)
[ProtoStax LCD Kit V2](https://www.protostax.com/products/protostax-lcd-kit-v2)
[ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/products/protostax-kit-for-momentary-push-button-switches)

## Prerequisites

This demo uses
* [ProtoStax for Arduino](https://www.protostax.com/products/protostax-for-arduino)
* [ProtoStax LCD Kit V2](https://www.protostax.com/products/protostax-lcd-kit-v2)
* [ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/products/protostax-for-arduino)
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
Install MmlMusic from GitHub
Install ProtoStax_MmlMusicPlayer from GitHub
```

Get the source code
```
git clone https://github.com/protostax/ProtoStax_Endless_Runner_Game.git
```

## Usage

See instructions in Tutorial linked above for putting together the circuit and
enclosure. 

```
open
ProtoStax_Endless_Runner_Game/ProtoStax_Endless_Runner_Game.ino in
Arduino IDE
compile sketch and upload to your Arduino
```

## License

Written by Sridhar Rajagopal for ProtoStax. BSD license, all text above must be included in any redistribution

A lot of time and effort has gone into providing this and other code. Please support ProtoStax by purchasing products from us!

