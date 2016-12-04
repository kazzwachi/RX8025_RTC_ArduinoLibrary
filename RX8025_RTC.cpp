#include "RX8025_RTC.h"
#include <Wire.h>
#include <TimeLib.h>

/********************************************************************************/
RX8025_RTC::RX8025_RTC(){
  
}

bool RX8025_RTC::write(tmElements_t tm){
  _year       = tmYearToCalendar(tm.Year);
  _month      = tm.Month;
  _day        = tm.Day;
  _hour       = tm.Hour;
  _minute     = tm.Minute;
  _second     = tm.Second;
  return _begin();  
}

tmElements_t RX8025_RTC::read(){
  tmElements_t tm;
  if(_read()){
    tm.Year = CalendarYrToTm(_year);
    tm.Month = _month;
    tm.Day = _day;
    tm.Hour = _hour;
    tm.Minute = _minute;
    tm.Second = _second;
    return tm;
  }
}

/********************************************************************************/
bool RX8025_RTC::_begin(){
  byte n;
  Wire.beginTransmission(RTC8052_I2CADDR);
  Wire.write(0xE0);
  Wire.write(RTC8052_CFG_PULSE_ITR_SEC);  //Control 1
  Wire.write(0x00);         //Control 2
  Wire.endTransmission();
  delay(1);

  Wire.beginTransmission(RTC8052_I2CADDR);
  Wire.write(0x00);
  n = _toBCD(_second);
  Wire.write(n);            //0:Seconds
  n = _toBCD(_minute); 
  Wire.write(n);            //1:Minutes
  n = _toBCD(_hour);
  Wire.write(n);            //2:Hours
  Wire.write(0x00);         //3:Weekdays
  n = _toBCD(_day);
  Wire.write(n);            //4:Days
  n = _toBCD(_month);
  Wire.write(n);            //5:Months
  n = _toBCD(_year);
  Wire.write(n);            //6:Years
  Wire.endTransmission();
  delay(1);
    
  Wire.beginTransmission(RTC8052_I2CADDR);
  Wire.write(0x8E);
  Wire.write(0x00);   //8:Alarm_W ; Minute
  Wire.write(0x00);   //9:Alarm_W ; Hour
  Wire.write(0x00);   //A:Alarm_W ; Weekday
  Wire.write(0x00);   //B:Alarm_D ; Minute
  Wire.write(0x00);   //C:Alarm_D ; Hour
  Wire.endTransmission();
  delay(1);
  
  return true;
}

bool RX8025_RTC::_read(){
  byte years,months,days,hours,minutes,seconds;

  Wire.requestFrom(RTC8052_I2CADDR, 8);
  Wire.read();
  seconds = Wire.read();
  minutes = Wire.read();
  hours = Wire.read();

/**
  Serial.print("debug-->");
  Serial.print(hours >> 4);
  Serial.print(hours & 0x0F);
  Serial.print(":");  
  Serial.print(minutes >> 4);
  Serial.print(minutes & 0x0F);
  Serial.print(":");
  Serial.print(seconds >> 4);
  Serial.print(seconds & 0x0F);
  Serial.println("");
**/

  Wire.read();
  days = Wire.read();
  months = Wire.read();
  years = Wire.read();

  _year = _fromBCD(years) + 2000;
  _month = _fromBCD(months);
  _day = _fromBCD(days);
  _hour = _fromBCD(hours);
  _minute = _fromBCD(minutes);
  _second = _fromBCD(seconds);

  return true;
}

/********************************************************************************/
byte RX8025_RTC::_toBCD(int i){
  byte n0,n1;
  n0 = i % 10;
  n1 = (i /10) % 10;
  return (n1 << 4) | n0;
}

int RX8025_RTC::_fromBCD(byte bcd){
  int i0,i1;
  i0 = bcd & 0x0F;
  i1 = (bcd >> 4) * 10;
  return i0 + i1;
}


