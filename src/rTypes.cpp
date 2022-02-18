#include "rTypes.h"
#include "time.h"

bool checkTimespan(struct tm* timeinfo, timespan_t timespan)
{
  if (timespan > 0) {
    int16_t  t0 = timeinfo->tm_hour * 100 + timeinfo->tm_min;
    uint16_t t1 = timespan / 10000;
    uint16_t t2 = timespan % 10000;

    // t1 < t2 :: ((t0 >= t1) && (t0 < t2))
    // t0=0559 t1=0600 t2=2300 : (0559 >= 0600) && (0559 < 2300) = 0 && 1 = 0
    // t0=0600 t1=0600 t2=2300 : (0600 >= 0600) && (0600 < 2300) = 1 && 1 = 1
    // t0=0601 t1=0600 t2=2300 : (0601 >= 0600) && (0601 < 2300) = 1 && 1 = 1
    // t0=2259 t1=0600 t2=2300 : (2259 >= 0600) && (2259 < 2300) = 1 && 1 = 1
    // t0=2300 t1=0600 t2=2300 : (2300 >= 0600) && (2300 < 2300) = 1 && 0 = 0
    // t0=2301 t1=0600 t2=2300 : (2301 >= 0600) && (2301 < 2300) = 1 && 0 = 0

    // t1 > t2 :: !((t0 >= t2) && (t1 > t0))
    // t0=2259 t1=2300 t2=0600 : (2259 >= 0600) && (2300 > 2259) = 1 && 1 = 1 !=> 0
    // t0=2300 t1=2300 t2=0600 : (2300 >= 0600) && (2300 > 2300) = 1 && 0 = 0 !=> 1
    // t0=2301 t1=2300 t2=0600 : (2301 >= 0600) && (2300 > 2301) = 1 && 0 = 0 !=> 1
    // t0=0559 t1=2300 t2=0600 : (0559 >= 0600) && (2300 > 0559) = 0 && 1 = 0 !=> 1
    // t0=0600 t1=2300 t2=0600 : (0600 >= 0600) && (2300 > 0600) = 1 && 1 = 1 !=> 0
    // t0=0601 t1=2300 t2=0600 : (0601 >= 0600) && (2300 > 0601) = 1 && 1 = 1 !=> 0

    return (t1 < t2) ? ((t0 >= t1) && (t0 < t2)) : !((t0 >= t2) && (t1 > t0));
  };
  return false;
}

bool checkTimespanTime(time_t time, timespan_t timespan)
{
  if ((time > 1000000000) && (timespan > 0)) {
    static struct tm ti; 
    localtime_r(&time, &ti);
    return checkTimespan(&ti, timespan);
  };
  return false;
}

bool checkTimespanTimeEx(time_t time, timespan_t timespan, bool in_range)
{
  if ((time > 1000000000) && (timespan > 0)) {
    static struct tm ti; 
    localtime_r(&time, &ti);
    if (in_range) {
      return checkTimespan(&ti, timespan);
    } else {
      return !checkTimespan(&ti, timespan);
    };
  };
  return false;
}

bool checkTimespanNow(timespan_t timespan)
{
  time_t now = time(nullptr);
  return checkTimespanTime(now, timespan);
}

bool checkTimespanNowEx(timespan_t timespan, bool in_range)
{
  time_t now = time(nullptr);
  return checkTimespanTimeEx(now, timespan, in_range);
}

bool checkWeekday(struct tm* timeinfo, weekdays_t day)
{
  switch (day) {
    case WEEK_SUNDAY:    return timeinfo->tm_wday == 0;
    case WEEK_MONDAY:    return timeinfo->tm_wday == 1;
    case WEEK_TUESDAY:   return timeinfo->tm_wday == 2;
    case WEEK_WEDNESDAY: return timeinfo->tm_wday == 3;
    case WEEK_THURSDAY:  return timeinfo->tm_wday == 4;
    case WEEK_FRIDAY:    return timeinfo->tm_wday == 5;
    case WEEK_SATURDAY:  return timeinfo->tm_wday == 6;
    case WEEK_WEEKDAYS:  return (timeinfo->tm_wday > 0) && (timeinfo->tm_wday < 6);
    case WEEK_WEEKEND:   return (timeinfo->tm_wday == 0) || (timeinfo->tm_wday == 6);
    case WEEK_ANY:       return true;
    default:             return false;
  };
}