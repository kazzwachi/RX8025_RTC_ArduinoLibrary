#ifndef _RX8025_RTC_H
#define _RX8025_RTC_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <TimeLib.h>

#define RTC8052_I2CADDR 0x32
#define RTC8052_CFG_PULSE_ITR_SEC 0b00100011  //0-23hours. 1HzPulse on INTRA 

class RX8025_RTC{
  public:
    RX8025_RTC();
    bool write(tmElements_t tm);
    tmElements_t read();
  private:
    bool _begin();
    bool _read();
    byte _toBCD(int i);
    int _fromBCD(byte bcd);
    int _year;   //offset from 2000
    int _month;
    int _day;
    int _hour;   //0-24
    int _minute;
    int _second;
    bool _configured;
};

#endif
