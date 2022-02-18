/* 
   EN: Unified type and struct declarations
   RU: Объявления общих типов и структур для остальных модулей
   --------------------------
   (с) 2021 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RTYPES_H__
#define __RTYPES_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_bit_defs.h"

/**
 * Callback function for controlling relays from various subroutines
 * */
typedef bool (*cb_relay_control_t) (bool relay_state);

/**
 * Callback function for posting data to MQTT brocker
 * */
typedef bool (*cb_mqtt_publish_t) (void* sender, char* topic, char* payload, uint8_t qos, bool retained, bool forced, bool free_topic, bool free_payload);


/**
 * Parameter kinds
 * */
typedef enum { 
  OPT_KIND_PARAMETER          = 0,   // Parameter or setting for this device
  OPT_KIND_PARAMETER_LOCATION = 1,   // Parameter or setting common to all location devices
  OPT_KIND_LOCDATA_ONLINE     = 2,   // External local input: online only
  OPT_KIND_LOCDATA_STORED     = 3,   // External local input: online, keeping the last value (can consume NVS pages a lot)
  OPT_KIND_COMMAND            = 4,   // Specialized: commands
  OPT_KIND_OTA                = 5    // Specialized: OTA command
} param_kind_t;

/**
 * Parameter types
 * */
typedef enum { 
  OPT_TYPE_UNKNOWN   = 0,
  OPT_TYPE_I8        = 1,
  OPT_TYPE_U8        = 2,
  OPT_TYPE_I16       = 3,
  OPT_TYPE_U16       = 4,
  OPT_TYPE_I32       = 5,
  OPT_TYPE_U32       = 6,
  OPT_TYPE_I64       = 7,
  OPT_TYPE_U64       = 8,
  OPT_TYPE_FLOAT     = 9,
  OPT_TYPE_DOUBLE    = 10,
  OPT_TYPE_STRING    = 11,
  OPT_TYPE_TIME      = 12,
  OPT_TYPE_TIMESPAN  = 13
} param_type_t;

/**
 * Result of a call to a third-party REST API
 * */
typedef enum { 
  API_OK             = 0, 
  API_ERROR          = 1,
  API_ERROR_HTTP     = 2
} api_status_t;

/**
 * Automatic shift of the range limits
 * */
typedef enum { 
  LIM_AUTOSHIFT_OFF  = 0,
  LIM_AUTOSHIFT_ALL  = 1,
  LIM_AUTOSHIFT_LOW  = 2,
  LIM_AUTOSHIFT_HIGH = 3
} limits_autoshift_t;

/**
 * Fire and Security System and RX433 Messages
 * */
typedef enum { 
  RTM_NONE           = 0,
  RTM_WIRED          = 1,  
  RTM_RS232          = 2,
  RTM_RS485          = 3,
  RTM_RX433          = 4,
  RTM_MQTT           = 5,
  RTM_TELEGRAM       = 6
} reciever_type_t;

typedef struct {
  reciever_type_t source;
  uint16_t address;
  uint32_t value;
  uint16_t count;
} reciever_data_t;  

typedef uint32_t timespan_t;
typedef enum {
  WEEK_EMPTY = 0,
  WEEK_SUNDAY,
  WEEK_MONDAY,
  WEEK_TUESDAY,
  WEEK_WEDNESDAY,
  WEEK_THURSDAY,
  WEEK_FRIDAY,
  WEEK_SATURDAY,
  WEEK_WEEKDAYS,
  WEEK_WEEKEND,
  WEEK_ANY
} weekdays_t;

#ifdef __cplusplus
extern "C" {
#endif

bool checkTimespan(struct tm* timeinfo, timespan_t timespan);
bool checkTimespanTime(time_t time, timespan_t timespan);
bool checkTimespanTimeEx(time_t time, timespan_t timespan, bool in_range);
bool checkTimespanNow(timespan_t timespan);
bool checkTimespanNowEx(timespan_t timespan, bool in_range);
bool checkWeekday(struct tm* timeinfo, weekdays_t day);

#ifdef __cplusplus
}
#endif

#endif // __RTYPES_H__

