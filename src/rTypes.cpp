#include "rTypes.h"
#include "time.h"
#include "math.h"

msg_options_t encMsgOptions(msg_kind_t kind, bool notify, msg_priority_t priority)
{
  return ((uint8_t)kind & 0x07) << 4 | ((uint8_t)priority & 0x07) << 1 | ((uint8_t)notify & 0x01);
}

bool decMsgOptionsNotify(msg_options_t options)
{
  return (options & 0x01) != 0;
}

msg_kind_t decMsgOptionsKind(msg_options_t options)
{
  return msg_kind_t((options >> 4) & 0x07);
}

msg_priority_t decMsgOptionsPriority(msg_options_t options)
{
  return msg_priority_t((options >> 1) & 0x07);
}

bool checkThreshold(float value, threshold_type_t type, float threshold, bool current_state)
{
  if (!isnan(value) && (!isnan(threshold))) {
    if (current_state) {
      if (type == THRESHOLD_MORE_OR_EQUAL) 
        return value > threshold;
      else if (type == THRESHOLD_MORE) 
        return value >= threshold;
      else if (type == THRESHOLD_LESS_OR_EQUAL) 
        return value < threshold;
      else if (type == THRESHOLD_LESS) 
        return value <= threshold;
    } else {
      if (type == THRESHOLD_MORE_OR_EQUAL) 
        return value >= threshold;
      else if (type == THRESHOLD_MORE) 
        return value > threshold;
      else if (type == THRESHOLD_LESS_OR_EQUAL) 
        return value <= threshold;
      else if (type == THRESHOLD_LESS) 
        return value < threshold;
    };
  };
  return false;
}

bool checkThresholdFloat(float value, threshold_float_t* threshold, bool current_state)
{
  if (threshold) {
    if (threshold->hysteresis_type == HYSTERESIS_SWITCH_ON) {
      // Hysteresis only for switching on
      return checkThreshold(value, threshold->threshold_type, 
        current_state 
          ? threshold->threshold  
          : ((threshold->threshold_type == THRESHOLD_MORE_OR_EQUAL) || (threshold->threshold_type == THRESHOLD_MORE)) ? 
            threshold->threshold + threshold->hysteresis : threshold->threshold - threshold->hysteresis,
        current_state);
    } else if (threshold->hysteresis_type == HYSTERESIS_SWITCH_OFF) {
      // Hysteresis only for switching off
      return checkThreshold(value, threshold->threshold_type, 
        current_state 
          ? ((threshold->threshold_type == THRESHOLD_MORE_OR_EQUAL) || (threshold->threshold_type == THRESHOLD_MORE)) ? 
            threshold->threshold - threshold->hysteresis : threshold->threshold + threshold->hysteresis 
          : threshold->threshold,
        current_state);
    } else if (threshold->hysteresis_type == HYSTERESIS_SYMMETRIC) {
      // Hysteresis for both switching on and switching off in half
      return checkThreshold(value, threshold->threshold_type,
        current_state 
          ? ((threshold->threshold_type == THRESHOLD_MORE_OR_EQUAL) || (threshold->threshold_type == THRESHOLD_MORE)) ? 
            threshold->threshold - (threshold->hysteresis / 2) : threshold->threshold + (threshold->hysteresis / 2) 
          : ((threshold->threshold_type == THRESHOLD_MORE_OR_EQUAL) || (threshold->threshold_type == THRESHOLD_MORE)) ? 
            threshold->threshold + (threshold->hysteresis / 2) : threshold->threshold - (threshold->hysteresis / 2),
        current_state);
    } else {
      // Hysteresis is ignored
      return checkThreshold(value, threshold->threshold_type, threshold->threshold, current_state);
    };
  };
  return false;
}

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
    struct tm ti; 
    localtime_r(&time, &ti);
    return checkTimespan(&ti, timespan);
  };
  return false;
}

bool checkTimespanTimeEx(time_t time, timespan_t timespan, bool in_range)
{
  if ((time > 1000000000) && (timespan > 0)) {
    struct tm ti; 
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

bool checkSchedule(struct tm* timeinfo, schedule_t* schedule)
{
  return (timeinfo) && (schedule) && checkWeekday(timeinfo, schedule->weekdays) && checkTimespan(timeinfo, schedule->timespan);
}

bool checkTimeInterval(time_t timestamp, uint32_t interval, timeintv_t dimension, bool expired)
{
  time_t now = time(nullptr); 
  if ((timestamp > 1000000000) && (now >= timestamp)) {
    time_t _interval = (time_t)interval;
    if (dimension == TI_MILLISECONDS) {
      _interval = _interval / 1000;
    } else if (dimension == TI_MINUTES) {
      _interval = _interval * 60;
    } else if (dimension == TI_HOURS) {
      _interval = _interval * 60 * 60;
    } else if (dimension == TI_DAYS) {
      _interval = _interval * 60 * 60 * 24;
    };
    if (expired) {
      return (now - timestamp) > _interval;
    } else {
      return (now - timestamp) <= _interval;
    };
  };
  return false;
}