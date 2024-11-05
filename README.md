![](https://badges.registry.platformio.org/packages/sam4uk/library/WD%20Easy.svg)

# WD.Easy

This library is intended to control watchdog modes of operation, so far only series microcontrollers `AVR`.

## Features

I did not write it in the style of a singleton class (the watchdog is one). No matter how many objects are created, they will all work with the same register. I set myself the task to make the library as easy as possible and can be used for any Arduino and not only.

The main requirement is that the __TIMER SHOULD START COUNTING EVEN BEFORE THE FUNCTION `setup` STARTS__ , and even better before the start of `main`.


## It seems to have worked

### Methods are available in the timer

| Method              | DESCRIPTION                        |result   |
|---------------------|------------------------------------|---------|
|||                                                                  |
|`setTimeOut(uint8_t)`|Sets the timer trigger interval     |`void`   |
|`setMode(uint8_t)`   |Sets the action when the timer is triggered|`void`   |
|`setTask(ptr*)`      |Set the interrupt handler function  |`void`   |
|||                                                                  |
|`getTimeOut()`       |Get the frequency of activations    |`uint8_t`|
|`getMode()`          |Get the timer mode                  |`uint8_t`|
|`isEnable()`         |Is the timer active?                |`bool`   |
|||                                                                  |
|`reset()`            |Start a new countdown               |`void`   |

#### Intervals

The table shows the parameters accepted by the method `setTimeOut(uint8_t)` and returns `getTimeOut()`.

|define  |time s| |
|--------|:---:|-|
|WD_15MS |0.015| |
|WD_30MS |0.030| |
|WD_60MS |0.060| |
|WD_120MS|0.120| |
|WD_250MS|0.250| |
|WD_500MS|0.500| |
|WD_1S   |1    | |
|WD_2S   |2    | |
|WD_4S   |4    | |
|WD_8S   |8    | |

_Note_ The time is approximate and not suitable for measuring exact intervals, since the watchdog timer is clocked by an internal low-latency oscillator that "floats" when the supply voltage and temperature change

#### Actions

|define          |DESCRIPTION|
|----------------|-|
|DISABLED        |The timer is stopped|
|INTERRUPT       |When triggered, the interrupt handler will be called|
|SYSTEMRESET     |When triggered, the microcontroller will be rebooted|
|INTERUPTANDRESET|When triggered, the interrupt handler will be called </br> after the same intervals, the controller will be rebooted|

#### Обробник переривань

By default, no interrupt handler is specified (`nullptr`).

## Examples

### Example one
```cpp
void WD_tick() {
Serial.print("WD_tick()");
Serial.println(millis());
}

WatchDogEasy WD(WatchDogEasy::WD_1S, WatchDogEasy::INTERRUPT, WD_tick);

void setup() { Serial.begin(115200); }
void loop() {}
```
Here we trigger the timer to fire every second, when it fires, an interrupt will be generated, which will be handled by the `WD_tick()` function.

In this example, the timer starts counting even before the `setup()` function starts. This is convenient if there is a risk of the controller freezing during initiation.

### Example two

```cpp
void WD_tick() {
Serial.print("WD_tick()");
Serial.println(millis());
}

WatchDogEasy WD;

void setup() {
Serial.begin(115200);
WD.setTimeOut(WatchDogEasy::WD_1S);
WD.setMode(WatchDogEasy::INTERRUPT);
WD.setTask(WD_tick);
}
void loop() {}
```

Everything is the same here, but the timer starts counting after the last setter.

All setters reset the timer to a new count. Getters do not (Be careful). During the execution of the getter, the time may coincide and the action programmed in the timer may be executed.

### Example three

```cpp
WatchDogEasy WD(WatchDogEasy::WD_1S, WatchDogEasy::SYSTEMRESET, nullptr);

int main(){
for(;;){

}
}
``` 

Here, the timer is initialized before entering `main()`. In my opinion, it's super convenient

## "System Requirements"

The timer will work on any Arduino and AVR series microcontrollers

An additional 2 bytes in the RAM will take up space in the flash 198 bytes


## The most important thing

### Installation

1. You can download a zip archive [WD.Easy v1.0](https://github.com/Sam4uk/WD.Easy/archive/refs/tags/v1.0.zip) And extract the contents to the desired directory
2. The library is available for installation through the Arduino IDE library manager.
3. You can download a zip archive [WD.Easy prerelise](https://github.com/Sam4uk/WD.Easy/archive/refs/heads/main.zip). This is a test branch with the most recent code, not debugged. It is set as the first item of this list

Need to connect 
```cpp
 #include <WD.Easy.hpp>
```
and do not forget to set the timer so that it does not reset the program
