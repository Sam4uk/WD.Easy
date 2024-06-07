/**
 * @file WD.Easy.hpp
 * @author Sam4uk
 * @brief
 * @version 1.0.2
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#if !defined(_WD_EASY_HPP_)
#define _WD_EASY_HPP_
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <Tools.Easy.h>

using void_func_void = void (*)(void);

void_func_void WatchDogEasyTask = nullptr;

struct WatchDogEasy {
  /**
   * @brief WD delay
   */
  enum {
    WD_15MS,  ///< WD delay 15ms
    WD_30MS,  ///< WD delay 30ms
    WD_60MS,  ///< WD delay 60ms
    WD_120MS, ///< WD delay 120ms
    WD_250MS, ///< WD delay 250ms
    WD_500MS, ///< WD delay 500ms
    WD_1S,    ///< WD delay 1s
#if defined(WDP3)
    WD_2S, ///< WD delay 2s
    WD_4S, ///< WD delay 4s
    WD_8S  ///< WD delay 8s
#endif
  };
  /**
   * @brief Construct a new Easy Watch Dog object
   *
   * @param time
   * @param mode
   * @param task
   */
  WatchDogEasy(uint8_t time, uint8_t mode, void_func_void task) {
    reset();
    setTimeOut(time);
    setMode(mode);
    setTask(task);
  };
  /**
   * @brief
   *
   */
  enum {
    DISABLED,        ///<
    INTERRUPT,       ///<
    SYSTEMRESET,     ///<
    INTERUPTANDRESET ///<
  };
  /**
   * @brief Construct a new Easy Watch Dog object
   *
   */
  WatchDogEasy() { reset(); };
  /**
   * @brief Get the Time Out
   *
   * @return uint8_t
   */
  inline uint8_t getTimeOut() {
    uint8_t res{};
    wbi(res, 00, rbi(_WD_CONTROL_REG, WDP0));
    wbi(res, 01, rbi(_WD_CONTROL_REG, WDP1));
    wbi(res, 02, rbi(_WD_CONTROL_REG, WDP2));
#if defined(WDP3)
    wbi(res, 03, rbi(_WD_CONTROL_REG, WDP3));
#endif
    return res;
  }
  inline unsigned long getMillis() {
    constexpr unsigned long _interval_{(2048 / 128) + 1};
    return (_interval_ << getTimeOut());
  }
  /**
   * @brief Set the Time Out
   *
   * @param time
   */
  inline void setTimeOut(uint8_t time) {
    uint8_t set{_WD_CONTROL_REG | _bv(_WD_CHANGE_BIT) | _bv(WDE)};
    wbi(set, WDP0, rbi(time, 00));
    wbi(set, WDP1, rbi(time, 01));
    wbi(set, WDP2, rbi(time, 02));
#if defined(WDP3)
    wbi(set, WDP3, rbi(time, 03));
#endif
    __asm__ __volatile__("cli" ::: "memory");
    reset();
    _WD_CONTROL_REG |= (_bv(_WD_CHANGE_BIT) | _bv(WDE));
    _WD_CONTROL_REG = set;
    __asm__ __volatile__("sei" ::: "memory");
    return;
  };
  /**
   * @brief
   *
   * @return true
   * @return false
   */
  inline bool isEnable() {
    bool res{};
    wbi(res, 00, rbi(_WD_CONTROL_REG, WDE));
    wbi(res, 00, rbi(_WD_CONTROL_REG, WDIE));
    return res;
  };
  /**
   * @brief Set the Mode object
   *
   * @param mode
   */
  inline void setMode(uint8_t mode) {
    uint8_t set{_WD_CONTROL_REG | _bv(_WD_CHANGE_BIT)};
    wbi(set, WDIE, rbi(mode, 00));
    wbi(set, WDE, rbi(mode, 01));
    __asm__ __volatile__("cli" ::: "memory");
    reset();
    _WD_CONTROL_REG |= (_bv(_WD_CHANGE_BIT));
    _WD_CONTROL_REG = set;
    __asm__ __volatile__("sei" ::: "memory");
    return;
  };
  /**
   * @brief Get the Mode object
   *
   * @return uint8_t
   */
  inline uint8_t getMode() {
    uint8_t res{};
    wbi(res, 00, rbi(WDTCSR, WDIE));
    wbi(res, 01, rbi(WDTCSR, WDE));
    return res;
  }
  /**
   * @brief
   *
   */
  inline void reset() {
    __asm__ __volatile__("wdr");
    return;
  }
  /**
   * @brief Set the Task object
   *
   * @param task
   */
  void setTask(void_func_void task) {
    reset();
    WatchDogEasyTask = task;
  }
};

ISR(WDT_vect) {
  if (nullptr != WatchDogEasyTask)
    WatchDogEasyTask();
}

#endif // _WD_EASY_HPP_