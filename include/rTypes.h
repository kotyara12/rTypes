/* 
   EN: Unified type and struct declarations
   RU: Обявления общих типов и структур для остальных модулей
   --------------------------
   (с) 2021 Разживин Александр | Razzhivin Alexander
   kotyara12@yandex.ru | https://kotyara12.ru | tg: @kotyara1971
*/

#ifndef __RTYPES_H__
#define __RTYPES_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Parameter kinds
 * */
typedef enum { 
  OPT_KIND_PARAMETER = 0, 
  OPT_KIND_OTA       = 1,
  OPT_KIND_COMMAND   = 2
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

typedef uint32_t timespan_t;

#ifdef __cplusplus
extern "C" {
#endif

bool checkTimespan(struct tm timeinfo, timespan_t timespan);
bool checkTimespanTime(time_t time, timespan_t timespan);
bool checkTimespanTimeEx(time_t time, timespan_t timespan, bool in_range);
bool checkTimespanNow(timespan_t timespan);
bool checkTimespanNowEx(timespan_t timespan, bool in_range);

#ifdef __cplusplus
}
#endif

#endif // __RTYPES_H__

