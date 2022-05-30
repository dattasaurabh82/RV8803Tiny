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

#include <time.h>
#include "RV8803Tiny.h"

// Parse the __DATE__ predefined macro to generate date defaults:
// __Date__ Format: MMM DD YYYY (First D may be a space if <10)
// <MONTH>
#define BUILD_MONTH_JAN ((__DATE__[0] == 'J') && (__DATE__[1] == 'a')) ? 1 : 0
#define BUILD_MONTH_FEB (__DATE__[0] == 'F') ? 2 : 0
#define BUILD_MONTH_MAR ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'r')) ? 3 : 0
#define BUILD_MONTH_APR ((__DATE__[0] == 'A') && (__DATE__[1] == 'p')) ? 4 : 0
#define BUILD_MONTH_MAY ((__DATE__[0] == 'M') && (__DATE__[1] == 'a') && (__DATE__[2] == 'y')) ? 5 : 0
#define BUILD_MONTH_JUN ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'n')) ? 6 : 0
#define BUILD_MONTH_JUL ((__DATE__[0] == 'J') && (__DATE__[1] == 'u') && (__DATE__[2] == 'l')) ? 7 : 0
#define BUILD_MONTH_AUG ((__DATE__[0] == 'A') && (__DATE__[1] == 'u')) ? 8 : 0
#define BUILD_MONTH_SEP (__DATE__[0] == 'S') ? 9 : 0
#define BUILD_MONTH_OCT (__DATE__[0] == 'O') ? 10 : 0
#define BUILD_MONTH_NOV (__DATE__[0] == 'N') ? 11 : 0
#define BUILD_MONTH_DEC (__DATE__[0] == 'D') ? 12 : 0
#define BUILD_MONTH BUILD_MONTH_JAN | BUILD_MONTH_FEB | BUILD_MONTH_MAR |     \
                        BUILD_MONTH_APR | BUILD_MONTH_MAY | BUILD_MONTH_JUN | \
                        BUILD_MONTH_JUL | BUILD_MONTH_AUG | BUILD_MONTH_SEP | \
                        BUILD_MONTH_OCT | BUILD_MONTH_NOV | BUILD_MONTH_DEC
// <DATE>
#define BUILD_DATE_0 ((__DATE__[4] == ' ') ? 0 : (__DATE__[4] - 0x30))
#define BUILD_DATE_1 (__DATE__[5] - 0x30)
#define BUILD_DATE ((BUILD_DATE_0 * 10) + BUILD_DATE_1)
// <YEAR>
#define BUILD_YEAR (((__DATE__[7] - 0x30) * 1000) + ((__DATE__[8] - 0x30) * 100) + \
                    ((__DATE__[9] - 0x30) * 10) + ((__DATE__[10] - 0x30) * 1))

// Parse the __TIME__ predefined macro to generate time defaults:
// __TIME__ Format: HH:MM:SS (First number of each is padded by 0 if <10)
// <HOUR>
#define BUILD_HOUR_0 ((__TIME__[0] == ' ') ? 0 : (__TIME__[0] - 0x30))
#define BUILD_HOUR_1 (__TIME__[1] - 0x30)
#define BUILD_HOUR ((BUILD_HOUR_0 * 10) + BUILD_HOUR_1)
// <MINUTE>
#define BUILD_MINUTE_0 ((__TIME__[3] == ' ') ? 0 : (__TIME__[3] - 0x30))
#define BUILD_MINUTE_1 (__TIME__[4] - 0x30)
#define BUILD_MINUTE ((BUILD_MINUTE_0 * 10) + BUILD_MINUTE_1)
// <SECOND>
#define BUILD_SECOND_0 ((__TIME__[6] == ' ') ? 0 : (__TIME__[6] - 0x30))
#define BUILD_SECOND_1 (__TIME__[7] - 0x30)
#define BUILD_SECOND ((BUILD_SECOND_0 * 10) + BUILD_SECOND_1)

RV8803Tiny::RV8803Tiny(void)
{
}

bool RV8803Tiny::begin()
{
    TinyMegaI2C.init();

    if (TinyMegaI2C.start(RV8803_ADDR, 0))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool RV8803Tiny::updateTime()
{
    /*----- WIP: Use this part to check TIME_HUNDREDTHS -----*/
    /* Based on that we have created the below conditionals  */
    //
    // Serial.println("\n--------------------------");
    // Serial.println("[IN LIB] updating time ...");
    // Serial.print("[IN LIB] TIME_HUNDREDTHS: ");
    // Serial.println(BCDtoDEC(_time[TIME_HUNDREDTHS]));
    // Serial.print("[IN LIB] TIME: ");
    // Serial.print(BCDtoDEC(_time[TIME_HOURS]));
    // Serial.print(":");
    // Serial.print(BCDtoDEC(_time[TIME_MINUTES]));
    // Serial.print(":");
    // Serial.println(BCDtoDEC(_time[TIME_SECONDS]));
    // bool update = false;
    /*------------------------------------------------------*/

    if (!readMultipleRegisters(RV8803_HUNDREDTHS, _time, TIME_ARRAY_LENGTH))
    {   
        // Something went wrong
        return (false);
    }

    if (BCDtoDEC(_time[TIME_HUNDREDTHS]) == 99 ||   /* if hundredths are at 99, read again to make sure we didn't accidentally skip a second/min */
        BCDtoDEC(_time[TIME_SECONDS]) == 59 ||      /* if seconds are at 59, read again to make sure we didn't accidentally skip a second/min */
        BCDtoDEC(_time[TIME_HUNDREDTHS]) == 0 || 
        BCDtoDEC(_time[TIME_SECONDS]) == 0 ||
        BCDtoDEC(_time[TIME_SECONDS]) > 60)
        {

        uint8_t tempTime[TIME_ARRAY_LENGTH];

        // If the reading for hundredths has rolled over, then our new data is correct, otherwise, we can leave the old data.
        if (BCDtoDEC(_time[TIME_HUNDREDTHS]) > BCDtoDEC(tempTime[TIME_HUNDREDTHS])) 
        {
            memcpy(_time, tempTime, TIME_ARRAY_LENGTH);
        }

        if (!readMultipleRegisters(RV8803_HUNDREDTHS, tempTime, TIME_ARRAY_LENGTH))
        {   
            // Something went wrong
            return (false);
        }
    }

    return true;
}

// Returns the time in hh:mm:ss
char *RV8803Tiny::stringTime()
{
    static char time[9]; //Max of hh:mm:ss with \0 terminator
    sprintf(time, "%02d:%02d:%02d", BCDtoDEC(_time[TIME_HOURS]), BCDtoDEC(_time[TIME_MINUTES]), BCDtoDEC(_time[TIME_SECONDS]));
    return (time);
}

//Returns the date in the DD/MM/YYYY format.
char *RV8803Tiny::stringDate()
{
    static char date[11]; //Max of dd/mm/yyyy with \0 terminator
    sprintf(date, "%02d/%02d/20%02d", BCDtoDEC(_time[TIME_DATE]), BCDtoDEC(_time[TIME_MONTH]), BCDtoDEC(_time[TIME_YEAR]));
    return (date);
}

uint8_t RV8803Tiny::getSeconds()
{
    return BCDtoDEC(_time[TIME_SECONDS]);
}

uint8_t RV8803Tiny::getMinutes()
{
    return BCDtoDEC(_time[TIME_MINUTES]);
}

uint8_t RV8803Tiny::getHours()
{
    return BCDtoDEC(_time[TIME_HOURS]);
}

uint8_t RV8803Tiny::getDate()
{
    return BCDtoDEC(_time[TIME_DATE]);
}

uint8_t RV8803Tiny::getWeekday()
{
    uint8_t tempWeekday = _time[TIME_WEEKDAY];
    tempWeekday = log(tempWeekday) / log(2);
    return tempWeekday;
}

uint8_t RV8803Tiny::getMonth()
{
    return BCDtoDEC(_time[TIME_MONTH]);
}

uint16_t RV8803Tiny::getYear()
{
    return BCDtoDEC(_time[TIME_YEAR]) + 2000;
}

uint8_t RV8803Tiny::nthdig(int n, uint8_t k)
{
    while (n--)
        k /= 10;
    return k % 10;
}

uint8_t* RV8803Tiny::currTimeAsArray()
{
    currTimeArray[0] = nthdig(1, getHours());
    currTimeArray[1] = nthdig(0, getHours());
    currTimeArray[2] = nthdig(1, getMinutes());
    currTimeArray[3] = nthdig(0, getMinutes());
    currTimeArray[4] = nthdig(1, getSeconds());
    currTimeArray[5] = nthdig(0, getSeconds());

    return currTimeArray;
}

//------------ Setting time ----------------//
// Set time and date/day registers of RV8803
// Set time and date/day registers of RV8803 (using data array)
bool RV8803Tiny::setTime(uint8_t *time, uint8_t len = 8)
{
    if (len != TIME_ARRAY_LENGTH)
        return false;
    // We use length - 1 as that is the length without the read-only hundredths register We also point to the second element in the time array as hundredths is read only
    return writeMultipleRegisters(RV8803_SECONDS, time + 1, len - 1);
}

bool RV8803Tiny::setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t weekday, uint8_t date, uint8_t month, uint16_t year)
{
    _time[TIME_SECONDS] = DECtoBCD(sec);
    _time[TIME_MINUTES] = DECtoBCD(min);
    _time[TIME_HOURS] = DECtoBCD(hour);
    _time[TIME_DATE] = DECtoBCD(date);
    _time[TIME_WEEKDAY] = 1 << weekday;
    _time[TIME_MONTH] = DECtoBCD(month);
    _time[TIME_YEAR] = DECtoBCD(year - 2000);

    return setTime(_time, TIME_ARRAY_LENGTH); //Subtract one as we don't write to the hundredths register
}

// Takes the time from the last build and uses it as the current time
// Works very well as an arduino sketch
bool RV8803Tiny::setToCompilerTime()
{
    _time[TIME_SECONDS] = DECtoBCD(BUILD_SECOND); // ** +14(approx) to compensate for delay in uploading time
    _time[TIME_MINUTES] = DECtoBCD(BUILD_MINUTE);
    _time[TIME_HOURS] = DECtoBCD(BUILD_HOUR);

    _time[TIME_MONTH] = DECtoBCD(BUILD_MONTH);
    _time[TIME_DATE] = DECtoBCD(BUILD_DATE);
    _time[TIME_YEAR] = DECtoBCD(BUILD_YEAR - 2000); //! Not Y2K (or Y2.1K)-proof :(

    // Calculate weekday (from here: http://stackoverflow.com/a/21235587)
    // 0 = Sunday, 6 = Saturday
    uint16_t d = BUILD_DATE;
    uint16_t m = BUILD_MONTH;
    uint16_t y = BUILD_YEAR;
    uint16_t weekday = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7 + 1;
    _time[TIME_WEEKDAY] = 1 << weekday;

    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setSeconds(uint8_t value)
{
    _time[TIME_SECONDS] = DECtoBCD(value);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setMinutes(uint8_t value)
{
    _time[TIME_MINUTES] = DECtoBCD(value);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setHours(uint8_t value)
{
    _time[TIME_HOURS] = DECtoBCD(value);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setDate(uint8_t value)
{
    _time[TIME_DATE] = DECtoBCD(value);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setMonth(uint8_t value)
{
    _time[TIME_MONTH] = DECtoBCD(value);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8803Tiny::setYear(uint16_t value)
{
    _time[TIME_YEAR] = DECtoBCD(value - 2000);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

//--------------------------------------------------------//
//----------- READ | WRIRE | OPERATION WRAPPERS ----------//
//--------------------------------------------------------//
uint8_t RV8803Tiny::BCDtoDEC(uint8_t val)
{
    return ((val / 0x10) * 10) + (val % 0x10);
}

//--- BCDtoDEC -- convert decimal to binary-coded decimal (BCD) ---//
uint8_t RV8803Tiny::DECtoBCD(uint8_t val)
{
    return ((val / 10) * 0x10) + (val % 10);
}

//--- WRITE OPERATIONS [ as laid out in datasheet 6.7 ] ---//
bool RV8803Tiny::writeRegister(uint8_t addr, uint8_t val)
{
    //  Master sends out the START condition, signifyig it'll be a write operation [ R/W bit is a 0 ]
    if (TinyMegaI2C.start(RV8803_ADDR, 0))
    { //  if Acknowledged from RV-8803
        //  Master sends out the Register Address to RV-8803
        if (TinyMegaI2C.write(addr))
        { //  if Acknowledged from RV-8803
            // Master sends out the Data to write to the specified address
            if (TinyMegaI2C.write(val))
            { //  if Acknowledged from RV-8803
                // Master sends out the STOP Condition.
                TinyMegaI2C.stop();
                return (true);
            }
            else
            {
                // Master sends out the STOP Condition.
                TinyMegaI2C.stop();
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

bool RV8803Tiny::writeMultipleRegisters(uint8_t addr, uint8_t *values, uint8_t len)
{
    //  Master sends out the START condition, signifyig it'll be a write operation [ R/W bit is a 0 ]
    if (TinyMegaI2C.start(RV8803_ADDR, 0))
    { //  if Acknowledged from RV-8803
        //  Master sends out the Register Address to RV-8803
        if (TinyMegaI2C.write(addr))
        { //  if Acknowledged from RV-8803
            // Master sends out **multiple Data to write to the specified address
            dataSentCounter = 0;
            for (uint8_t i = 0; i < len; i++)
            {
                if (TinyMegaI2C.write(values[i]))
                { // if Acknowledged from RV-8803
                    dataSentCounter++;
                }
            }
            if (dataSentCounter == len)
            { // means all the data were transmitted and was acknowledged successfully
                // Master sends out the STOP Condition.
                TinyMegaI2C.stop();
                return (true);
            }
            else
            {
                // Master sends out the STOP Condition.
                TinyMegaI2C.stop();
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

//--- READ OPERATIONS [ as laid out in datasheet 6.8 ]  ---//

uint8_t RV8803Tiny::readRegister(uint8_t addr)
{
    //  Master sends out the START condition, signifyig it'll be a write operation [ R/W bit is a 0 ]
    if (TinyMegaI2C.start(RV8803_ADDR, 0))
    { //  if Acknowledged from RV-8803
        //  Master sends out the Register Address to RV-8803-C7.
        if (TinyMegaI2C.write(addr))
        { //  if Acknowledged from RV-8803
            //  Master sends out the Repeated START condition (or STOP condition followed by START condition)
            //  Master sends out Slave Address, 65h for the RV-8803 [ R/W bit is 1 ] indicating a read operation
            if (TinyMegaI2C.restart(RV8803_ADDR, 1))
            { //Acknowledgement from RV-8803-C7
                // The Slave sends out the Data from the Register Address
                uint8_t res = TinyMegaI2C.read();
                // 1. [NO NEED] Acknowledgement from Master.
                // 2. [NO NEED] The above Steps can be repeated if necessary and the address bit increments automatically.
                // 3. ** The Master, addressed as Receiver, can stop data transmission by not generating an acknowledge on the
                // last byte that has been sent from the Slave {thaat's why we didn't do step 1 here}.
                // In this event, the Slave-Transmitter must leave the data line HIGH to enable the Master to generate a STOP condition.
                TinyMegaI2C.stop();
                return (res);
            }
            else
            {
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}

bool RV8803Tiny::readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len)
{
    //  Master sends out the START condition, signifyig it'll be a write operation [ R/W bit is a 0 ]
    if (TinyMegaI2C.start(RV8803_ADDR, 0))
    { //  if Acknowledged from RV-8803
        //  Master sends out the Register Address to RV-8803-C7.
        if (TinyMegaI2C.write(addr))
        { //  if Acknowledged from RV-8803
            //  Master sends out the Repeated START condition (or STOP condition followed by START condition)
            //  Master sends out Slave Address, 65h for the RV-8803 [ R/W bit is -1 ] indicating unknown number of read operation
            if (TinyMegaI2C.restart(RV8803_ADDR, -1))
            { //Acknowledgement from RV-8803-C7
                // The Slave sends out the Data from the Register Address
                // 1. [NO NEED] Acknowledgement from Master.
                // 2. [NO NEED] The above Steps can be repeated if necessary and the address bit increments automatically.
                // 3. ** The Master, addressed as Receiver, can stop data transmission by not generating an acknowledge on the
                // last byte that has been sent from the Slave {thaat's why we didn't do step 1 here}.
                // In this event, the Slave-Transmitter must leave the data line HIGH to enable the Master to generate a STOP condition.

                for (uint8_t i = 0; i < len; i++)
                {
                    dest[i] = TinyMegaI2C.read();
                }

                TinyMegaI2C.stop();
                return (dest);
            }
            else
            {
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}
