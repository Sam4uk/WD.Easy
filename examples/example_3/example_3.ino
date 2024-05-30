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

WatchDogEasy WD(WatchDogEasy::WD_1S, WatchDogEasy::SYSTEMRESET, nullptr);

int main() {
  for (;;) {
  }
}
