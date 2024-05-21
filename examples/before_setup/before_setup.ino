/**
 * @file before_setup.ino
 * @author Sam4uk
 * @brief
 * @version 0.1
 * @date 2024-05-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <WD.Easy.hpp>

void WD_tick() {
  Serial.print("WD_tick()");
  Serial.println(millis());
}

WatchDogEasy WD(WatchDogEasy::WD_1S, WatchDogEasy::INTERRUPT, WD_tick);

void setup() { Serial.begin(115200); }
void loop() {}
