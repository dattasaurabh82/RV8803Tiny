/******************************************************************************
  RV8803Tiny.h
  RV8803 Library for TinyMegaCore based ATTINY Atmel chips.
  Saurabh Datta


  Resources & Dependencies:
  https://github.com/dattasaurabh82/TinyMegaI2C


  Development environment specifics:
  Arduino IDE:  1.8.13
  uC:           ATTINY-1607
******************************************************************************/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <TinyMegaI2C.h>
// #include <TinyMegaI2CMaster.h>

//The 7-bit I2C address of the RV8803-C7-32.768KHZ-3PPM-TA-QC
#define RV8803_ADDR 0x32

//Register names:
#define RV8803_RAM 0x07
#define RV8803_HUNDREDTHS 0x10
#define RV8803_SECONDS 0x11
#define RV8803_MINUTES 0x12
#define RV8803_HOURS 0x13
#define RV8803_WEEKDAYS 0x14
#define RV8803_DATE 0x15
#define RV8803_MONTHS 0x16
#define RV8803_YEARS 0x17

#define SUNDAY 0x01
#define MONDAY 0x02
#define TUESDAY 0x04
#define WEDNESDAY 0x08
#define THURSDAY 0x10
#define FRIDAY 0x20
#define SATURDAY 0x40

#define RV8803_TIMER_0 0x1B
#define RV8803_TIMER_1 0x1C

#define RV8803_EXTENSION 0x1D
#define RV8803_FLAG 0x1E

#define RV8803_CONTROL 0x1F
#define RV8803_HUNDREDTHS_CAPTURE 0x20
#define RV8803_SECONDS_CAPTURE 0x21
#define RV8803_OFFSET 0x2C
#define RV8803_EVENT_CONTROL 0x2F

#define TIME_ARRAY_LENGTH 8 // Total number of writable values in device

enum time_order
{
    TIME_HUNDREDTHS, // 0
    TIME_SECONDS,    // 1
    TIME_MINUTES,    // 2
    TIME_HOURS,      // 3
    TIME_WEEKDAY,    // 4
    TIME_DATE,       // 5
    TIME_MONTH,      // 6
    TIME_YEAR,       // 7
};

class RV8803Tiny
{
public:
    RV8803Tiny(void);
    bool begin();

    char *stringDate(); //Return date in dd-mm-yyyy
    char *stringTime(); //Return time hh:mm:ss in 24 hr mode

    uint8_t getSeconds();
    uint8_t getMinutes();
    uint8_t getHours();
    uint8_t getDate();
    uint8_t getWeekday();
    uint8_t getMonth();
    uint16_t getYear();

    bool updateTime(); //Update the local array with the RTC registers

    //-- ORIGINAL APPROACH --//
    // uint8_t currTimeArray[6];
    // updateTimeArray(void);
    //-----------------------//

    //-- NEW TEST ADDITION --//
    uint8_t* currTimeAsArray();
    uint8_t* currDateAsArray();
    //-----------------------//

    bool setToCompilerTime();

    bool setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year);
    bool setTime(uint8_t *time, uint8_t len);

    bool setSeconds(uint8_t value);
    bool setMinutes(uint8_t value);
    bool setHours(uint8_t value);
    bool setDate(uint8_t value);
    bool setWeekday(uint8_t value);
    bool setMonth(uint8_t value);
    bool setYear(uint16_t value);

    uint8_t readRegister(uint8_t addr);
    bool writeRegister(uint8_t addr, uint8_t val);
    bool readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len);
    bool writeMultipleRegisters(uint8_t addr, uint8_t *values, uint8_t len);

    // Values in RTC are stored in Binary Coded Decimal. These functions convert to/from Decimal
    uint8_t BCDtoDEC(uint8_t val);
    uint8_t DECtoBCD(uint8_t val);

private:
    uint8_t currTimeArray[6];
    uint8_t currDateArray[6];
    uint16_t currDecade = 2000; // update it in 978 years when we step in 3000's :)
    uint8_t _time[TIME_ARRAY_LENGTH];
    uint8_t dataSentCounter;
    uint8_t nthdig(int n, uint8_t k);
};
