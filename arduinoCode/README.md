# Arduino Code of 2019-2020

This folder acts as the store for all the programs of used on the Arduino baords.

Hardware used:
 - Arduino boards
   - Arduino AVR Uno R3
   - Arduino AVR Mega 2560
 - Arduino Shields
   - Adafruit Motorshield v2
   - Adafruit 16-Channel Servo shield
 - Generic DC motors
 - SG90 Mini servos

## Prorgam descriptions

[//]: # (This is a mix of HTML and MarkDown, make sure to put a blank line between HTML and MarkDown)
<details>
  <summary> multipleButtonPushersWithWheels </summary>

  > **Hardware**: uses Uno/Mega, motorshield, two DC motors, four servos
  > **Setup**: Connect to motorshield, attaches servos to board's pins, sets servos to resting position
  > **Loop**: runs only once, spins motors for a second, triggers servos in a specific order, individually servos: spin to push, wait a second, then pull back
  > **Link**: [here][multipleButtonPushersWithWheels]
</details>

<details>
  <summary> PointyShark </summary>

  > **Hardware**: uses Uno/Mega, 
  > **Setup**: 
  > **Loop**: 
  > **Link**: [here][PointyShark]
</details>

<details>
  <summary> robot230 </summary>

  > **Hardware**: uses Uno/Mega, 
  > **Setup**: 
  > **Loop**: 
  > **Link**: [here][robot230]
</details>

<details>
  <summary> servoShieldAndWheels </summary>

  > **Hardware**: uses Uno/Mega, 
  > **Setup**: 
  > **Loop**: 
  > **Link**: [here][servoShieldAndWheels]
</details>

<details>
  <summary> ServoTest </summary>

  > **Hardware**: uses Uno/Mega, 
  > **Setup**: 
  > **Loop**: 
  > **Link**: [here][ServoTest]
</details>

<details>
  <summary> servoWithWheels </summary>

  > **Hardware**: uses Uno/Mega, 
  > **Setup**: 
  > **Loop**: 
  > **Link**: [here][servoWithWheels]
</details>


[//]: # (Below are the relative links of all the referenced files)
[multipleButtonPushersWithWheels]: /multipleButtonPushersWithWheels
[PointyShark]: /PointyShark
[robot230]: /robot230
[servoShieldAndWheels]: /servoShieldAndWheels
[ServoTest]: /ServoTest
[servoWithWheels]: /servoWithWheels

## Helpful links
- [Arduino plugin for Visual Studio Code][Arduino VS Code Plugin]
  - Explains plugin abilties: **highlighting, intellisense, program the board, view serial monitor, send data through serial monitor, library installer**
- How to set up or use Arduino VS Code plugin
  - How to set up [Arduino intellisense][Arduino VS Code Intellisense]
    - I recommend keeping the **IntellisenseEngine** as default, but enable the **fallback** 
  - Step by step how to [program board from VS Code][Arduino VS Code Board Programming]
- How to use [Arduino programmer from command line][Arduino Cli Board Programming] (_good for programming from a pi_)
- How [Arduino builds and compiles code][Arduino Build Process], may be useful to figure out odd details with libraries, complication order, or used keywords

[//]: # (Below are all the referenced links)
[Arduino VS Code Plugin]: https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino
[Arduino VS Code Intellisense]: https://cuneyt.aliustaoglu.biz/en/enabling-arduino-intellisense-with-visual-studio-code/
[Arduino VS Code Board Programming]: https://www.dmcinfo.com/latest-thinking/blog/id/9484/arduino-programming-with-vscode
[Arduino Cli Board Programming]: https://www.woolseyworkshop.com/2019/04/14/using-the-arduino-command-line/
[Arduino Build Process]: https://github.com/arduino/Arduino/wiki/Build-Process