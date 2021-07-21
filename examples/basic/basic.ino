#include <RV8803Tiny.h>
RV8803Tiny rtc;

bool rtcAvailable;
bool rtcReadable;

int sec = 0;
int minute = 10;
int hour = 0; // Set value in 24 hour mode
int date = 30;
int month = 6;
int year = 2021;
int weekday = 3;

unsigned long startCountMillis;
unsigned long currentCountMillis;
const unsigned long secPeriod = 1000; //the value is a number of milliseconds

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  }

  while (rtc.begin() == false)
  { // un-shifted default address for RV-8803 is 0x32. Check library
    rtcAvailable = false;
    Serial.println(F("Something went wrong, ctheck wiring"));
    delay(2000);
  }
  rtcAvailable = true;

  // ---- Set the time for the RTC as the time during code compilation ---- //
  Serial.println(F("Setting to compiler time..."));
  if (rtc.setToCompilerTime() == false)
  {
    Serial.println("Something went wrong setting the time");
    while (true)
    {
      delay(2000);
    }
  }
  Serial.println(F("Compiler Time set!"));

  // ---- OR custom time setting method ---- //
  //  Serial.println(F("Setting custom time..."));
  //  if (rtc.setTime(sec, minute, hour, weekday, date, month, year) == false) {
  //    Serial.println("Something went wrong setting the time");
  //    while (true) {
  //      delay(2000);
  //    }
  //  }
  //  Serial.println(F("Custom Time set!"));
}

void loop()
{
  if (rtcAvailable)
  {
    // updateTime i.e read registers, ** must for getting current time
    if (rtc.updateTime())
    {
      Serial.print(rtc.getDate());
      Serial.print("/");
      Serial.print(rtc.getMonth());
      Serial.print("/");
      Serial.println(rtc.getYear());

      Serial.print("WEEKDAY: ");
      Serial.println(rtc.getWeekday());

      Serial.print(rtc.getHours());
      Serial.print(":");
      Serial.print(rtc.getMinutes());
      Serial.print(":");
      Serial.println(rtc.getSeconds());
      Serial.println("");
      /*
        .updateTimeArray(); // To push time read from registers to an int array
      */
    }
  }
  delay(1000);
}
