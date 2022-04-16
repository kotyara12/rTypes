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
 * Type of input data stream
 * */
typedef enum { 
  IDS_NONE           = 0,
  IDS_GPIO           = 1,  
  IDS_RX433          = 2,
  IDS_MQTT           = 3
} source_type_t;

/**
 * GPIO pin and logic level details :: 32 bit
 * */
typedef struct {
  uint8_t bus;      // I2C bus +1 for expanders, 0 for internal ports
  uint8_t address;  // I2C address for expanders, 0 for internal ports
  uint8_t pin;      // Chip pin number
  uint8_t value;    // Logic level: 0 or 1
} gpio_data_t;

/**
 * External data :: 40 bit
 * */
typedef struct {
  uint32_t id;       // External data ID
  uint8_t  value;    // Logic level: 0 or 1
} ext_data_t;


/**
 * Data packet from receiver RX433 :: 40 bit
 * */
typedef struct {
  uint8_t  protocol;
  uint32_t value;
} rx433_data_t;

/**
 * Data in mixed input stream :: 40+8+16 = 64 bit
 * */
typedef struct {
  source_type_t source;
  union {
    gpio_data_t  gpio;
    rx433_data_t rx433;
    ext_data_t   ext;
  };
  uint16_t count;
} input_data_t;  

/**
 * Callback function to change the logic level on the input GPIO
 * */
typedef void (*cb_gpio_change_t) (void* source, gpio_data_t data, uint32_t duration);

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
  OPT_KIND_EXTDATA_ONLINE     = 4,   // External data: fixed topic, online only
  OPT_KIND_EXTDATA_STORED     = 5,   // External data: fixed topic, keeping the last value (can consume NVS pages a lot)
  OPT_KIND_COMMAND            = 6,   // Specialized: commands
  OPT_KIND_OTA                = 7    // Specialized: OTA command
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
  OPT_TYPE_TIMEVAL   = 12,
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

