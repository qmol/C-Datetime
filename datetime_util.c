#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "datetime_util.h"

/* Function to validate following ISO 8601 formats:
    YYYY-MM-DDThh:mm:ss
    YYYY-MM-DDThh:mm:ssZ
    YYYY-MM-DDThh:mm:ss+hh:mm
    YYYY-MM-DDThh:mm:ss-hh:mm
    */


FunctionStatus validate_iso8601(const char* datetime)
{
    // check string length
    size_t dtstr_len = strlen(datetime);
    if (dtstr_len < 19) return DT_ERR_TOO_SHORT;
    
    // Check YYYY (year)
    for (int i = 0; i < 4; i++) {
        if (!isdigit(datetime[i])) return DT_ERR_WRONG_YEAR;
    }
    
    // Check first dash
    if (datetime[4] != '-') return DT_ERR_WRONG_SEPARATOR;
    
    // Check MM (month)
    if (!isdigit(datetime[5]) || !isdigit(datetime[6])) return DT_ERR_WRONG_MONTH;
    uint8_t month = (datetime[5] - '0') * 10 + (datetime[6] - '0');
    if (month < 1 || month > 12) return DT_ERR_WRONG_MONTH;
    
    // Check second dash
    if (datetime[7] != '-') return DT_ERR_WRONG_SEPARATOR;
    
    // Check DD (day)
    if (!isdigit(datetime[8]) || !isdigit(datetime[9])) return DT_ERR_WRONG_DAY;
    uint8_t day = (datetime[8] - '0') * 10 + (datetime[9] - '0');
    if (day < 1 || day > 31) return DT_ERR_WRONG_DAY;
    
    // Check T separator
    if (datetime[10] != 'T') return DT_ERR_WRONG_SEPARATOR;
    
    // Check hh (hour)
    if (!isdigit(datetime[11]) || !isdigit(datetime[12])) return DT_ERR_WRONG_HOUR;
    uint8_t hour = (datetime[11] - '0') * 10 + (datetime[12] - '0');
    if ( hour > 23) return DT_ERR_WRONG_HOUR;
    
    // Check first colon
    if (datetime[13] != ':') return DT_ERR_WRONG_SEPARATOR;
    
    // Check mm (minute)
    if (!isdigit(datetime[14]) || !isdigit(datetime[15])) return DT_ERR_WRONG_MINUTE;
    uint8_t minute = (datetime[14] - '0') * 10 + (datetime[15] - '0');
    if ( minute > 59) return DT_ERR_WRONG_MINUTE;
    
    // Check second colon
    if (datetime[16] != ':') return DT_ERR_WRONG_SEPARATOR;
    
    // Check ss (second)
    if (!isdigit(datetime[17]) || !isdigit(datetime[18])) return DT_ERR_WRONG_SECOND;
    uint8_t second = (datetime[17] - '0') * 10 + (datetime[18] - '0');
    if ( second > 59) return DT_ERR_WRONG_SECOND;
    
    // Check timezone designator (TZD)
    if (dtstr_len == 19) {
        // No timezone specified - local time
        return RET_SUCCESS;
    }
    
    // Check for Z (UTC) 
    if (dtstr_len == 20 && datetime[19] == 'Z') {
        return RET_SUCCESS;
    }

    // Check for timezone offset +/-HH:MM
    if (dtstr_len >= 25 && (datetime[19] == '+' || datetime[19] == '-')) {
        // Check timezone format +/-HH:MM
        if (!isdigit(datetime[20]) || !isdigit(datetime[21])) return DT_ERR_WRONG_TZ_HOUR;
        if (datetime[22] != ':') return DT_ERR_WRONG_SEPARATOR;
        if (!isdigit(datetime[23]) || !isdigit(datetime[24])) return DT_ERR_WRONG_TZ_MINUTE;
        
        int8_t tz_hour = (datetime[20] - '0') * 10 + (datetime[21] - '0');
        int8_t tz_minute = (datetime[23] - '0') * 10 + (datetime[24] - '0');
        
        if (tz_hour > 14) return DT_ERR_WRONG_TZ_HOUR;
        if (tz_minute > 59) return DT_ERR_WRONG_TZ_MINUTE;
        return RET_SUCCESS;
    }

    return DT_ERR_WRONG_TZ;
}

/* Fucntion to validate and normalize date time value
    For local date-time, copy directly.
    All other time converts to GMT by adjusting the time based on the timezone offset
*/
FunctionStatus normalize_iso8601(const char* datetime, char datetime_utc[25])
{
    datetime_utc[0] = '\0';

    // check string length
    size_t dtstr_len = strlen(datetime);
    if (dtstr_len < 19) return DT_ERR_TOO_SHORT;
    
    // Check YYYY (year)
    for (int i = 0; i < 4; i++) {
        if (!isdigit(datetime[i])) return DT_ERR_WRONG_YEAR;
    }
    int16_t year =  (datetime[0] - '0') * 1000 + 
                (datetime[1] - '0') * 100 + 
                (datetime[2] - '0') * 10 + 
                (datetime[3] - '0');
    
    // Check first dash
    if (datetime[4] != '-') return DT_ERR_WRONG_SEPARATOR;
    
    // Check MM (month)
    if (!isdigit(datetime[5]) || !isdigit(datetime[6])) return DT_ERR_WRONG_MONTH;
    int8_t month = (datetime[5] - '0') * 10 + (datetime[6] - '0');
    if (month < 1 || month > 12) return DT_ERR_WRONG_MONTH;
    
    // Check second dash
    if (datetime[7] != '-') return DT_ERR_WRONG_SEPARATOR;
    
    // Check DD (day)
    if (!isdigit(datetime[8]) || !isdigit(datetime[9])) return DT_ERR_WRONG_DAY;
    int8_t day = (datetime[8] - '0') * 10 + (datetime[9] - '0');
    if (day < 1 || day > 31) return DT_ERR_WRONG_DAY;
    
    // Check T separator
    if (datetime[10] != 'T') return DT_ERR_WRONG_SEPARATOR;
    
    // Check hh (hour)
    if (!isdigit(datetime[11]) || !isdigit(datetime[12])) return DT_ERR_WRONG_HOUR;
    int8_t hour = (datetime[11] - '0') * 10 + (datetime[12] - '0');
    if (hour < 0 || hour > 23) return DT_ERR_WRONG_HOUR;
    
    // Check first colon
    if (datetime[13] != ':') return DT_ERR_WRONG_SEPARATOR;
    
    // Check mm (minute)
    if (!isdigit(datetime[14]) || !isdigit(datetime[15])) return DT_ERR_WRONG_MINUTE;
    int8_t minute = (datetime[14] - '0') * 10 + (datetime[15] - '0');
    if (minute < 0 || minute > 59) return DT_ERR_WRONG_MINUTE;
    
    // Check second colon
    if (datetime[16] != ':') return DT_ERR_WRONG_SEPARATOR;
    
    // Check ss (second)
    if (!isdigit(datetime[17]) || !isdigit(datetime[18])) return DT_ERR_WRONG_SECOND;
    int8_t second = (datetime[17] - '0') * 10 + (datetime[18] - '0');
    if (second < 0 || second > 59) return DT_ERR_WRONG_SECOND;
    
    // Check timezone designator (TZD)
    if (dtstr_len == 19) {
        // No timezone specified - local time
        strncpy(datetime_utc, datetime, 19);
        datetime_utc[19] = '\0';
        return RET_SUCCESS;
    }
    
    // Check for Z (UTC)
    if (dtstr_len == 20 && datetime[19] == 'Z') {
        // No timezone specified - local time
        strncpy(datetime_utc, datetime, 20);
        datetime_utc[20] = '\0';
        return RET_SUCCESS;
    }

    // Check for timezone offset +/-HH:MM
    if (dtstr_len >= 25 && (datetime[19] == '+' || datetime[19] == '-')) {
        // Check timezone format +/-HH:MM
        if (!isdigit(datetime[20]) || !isdigit(datetime[21])) return DT_ERR_WRONG_TZ_HOUR;
        if (datetime[22] != ':') return DT_ERR_WRONG_SEPARATOR;
        if (!isdigit(datetime[23]) || !isdigit(datetime[24])) return DT_ERR_WRONG_TZ_MINUTE;
        
        int8_t tz_hour = (datetime[20] - '0') * 10 + (datetime[21] - '0');
        int8_t tz_minute = (datetime[23] - '0') * 10 + (datetime[24] - '0');
        
        if (tz_hour > 14) return DT_ERR_WRONG_TZ_HOUR;
        if (tz_minute > 59) return DT_ERR_WRONG_TZ_MINUTE;
        
        // Adjust time to GMT
        if (datetime[19] == '+') {
            hour -= tz_hour;
            minute -= tz_minute;
        } else {
            hour += tz_hour;
            minute += tz_minute;
        }

        if (minute < 0) {
            minute += 60;
            hour -= 1;
        } else if (minute >= 60) {
            minute -= 60;
            hour += 1;
        }

        if (hour < 0) {
            hour += 24;
            day -= 1;
            // Note: This does not handle month/year underflow for simplicity
        } else if (hour >= 24) {
            hour -= 24;
            day += 1;
            // Note: This does not handle month/year overflow for simplicity
        } 

        // =========================================
        //  !!!  No semantic checking for dates  !!!
        // =========================================
        if (day <= 0) {
            day += 31;
            month -= 1;
        } else if (day > 31) {
            day -= 31;
            month += 1;
        }

        if (month <= 0) {
            month += 12;
            year -= 1;
        } else if (month > 12) {
            month -= 12;
            year += 1;
        }

        if (year < 0 || year > 9999) return DT_ERR_WRONG_YEAR; // Year under/over-flow, not supported

        sprintf(datetime_utc, "%04d-%02d-%02dT%02d:%02d:%02dZ", 
                year, month, day, hour, minute, second);
        return RET_SUCCESS;
    }
    
    return DT_ERR_WRONG_TZ;
}

void test_validator()
{
    struct test_data_struct
    {
        const char* datetime;
        FunctionStatus expected; 
    };

    struct test_data_struct  test_cases[] = {
        {"2023-10-05t14:30:00",         DT_ERR_WRONG_SEPARATOR},
        {"2023-10-05T14:30:00",         RET_SUCCESS},
        {"2023-10-05T14:30:00Z",        RET_SUCCESS},
        {"2023-10-05T14:30:00+14:00",   RET_SUCCESS},
        {"2023-10-05T14:30:00-05:00",   RET_SUCCESS},
        {"2023-10-05",                  DT_ERR_TOO_SHORT},
        {"2023-10-05T14:30",            DT_ERR_TOO_SHORT},
        {"2023-10-05T14:30:00+2:00",    DT_ERR_WRONG_TZ},
        {"2023-10-05T14:30:00+02:0",    DT_ERR_WRONG_TZ},
        {"2023-10-05T14:30:00+15:00",   DT_ERR_WRONG_TZ_HOUR},
        {"2023-10-05T14:30:00+02:60",   DT_ERR_WRONG_TZ_MINUTE},
        {"2023-13-05T14:30:00",         DT_ERR_WRONG_MONTH}, 
        {"2023-10-32T14:30:00",         DT_ERR_WRONG_DAY}, 
        {"2023-10-05T24:30:00",         DT_ERR_WRONG_HOUR}, 
        {"2023-10-05T14:60:00",         DT_ERR_WRONG_MINUTE}, 
        {"2023-10-05T14:30:60",         DT_ERR_WRONG_SECOND},
        {"2023-10-05T14:30:00+14:00",   RET_SUCCESS}, 
        {"2023-10-05T14:30:00-14:00",   RET_SUCCESS}, 
        {"2023-10-05T14:30:00+00:00",   RET_SUCCESS}, 
        {"2023-10-05T14:30:00-00:00",   RET_SUCCESS}, 
        {"2023-10-05 14:30:00Z",        DT_ERR_WRONG_SEPARATOR},
        {"InvalidString",               DT_ERR_TOO_SHORT}
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed = 0;
    for (int i = 0; i < num_tests; i++) {
        FunctionStatus result = validate_iso8601(test_cases[i].datetime);
        if (result == test_cases[i].expected) {
            printf("Test %d passed: %s\n", i + 1, test_cases[i].datetime);
            passed++;
        } else {
            printf("Test %d failed: %s (expected %d, got %d)\n", 
                   i + 1, test_cases[i].datetime, test_cases[i].expected, result);
        }
    }
    printf("%d/%d validation tests passed.\n", passed, num_tests);

}

void test_normalizer()
{
    struct test_data_struct
    {
        const char* datetime;
        char expected[25]; 
        FunctionStatus ret_status;
    };

    struct test_data_struct  test_cases[] = {
        {"2023-10-05T14:30:00",         "2023-10-05T14:30:00",       RET_SUCCESS},
        {"2023-10-05T14:30:00Z",        "2023-10-05T14:30:00Z",      RET_SUCCESS},
        {"2023-10-05T14:30:00+02:00",   "2023-10-05T12:30:00Z",      RET_SUCCESS},
        {"2023-10-05T14:30:00-05:00",   "2023-10-05T19:30:00Z",      RET_SUCCESS},
        {"2023-01-01T01:30:00+02:00",   "2022-12-31T23:30:00Z",      RET_SUCCESS},
        {"InvalidString",               "",                          DT_ERR_TOO_SHORT}
    };

    char datetime_utc[25];

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed = 0;
    for (int i = 0; i < num_tests; i++) {
        FunctionStatus result = normalize_iso8601(test_cases[i].datetime, datetime_utc);
        if (result == test_cases[i].ret_status &&
            strcmp(datetime_utc, test_cases[i].expected) == 0) {
            printf("Test %d passed: %s\n", i + 1, test_cases[i].datetime);
            passed++;
        } else {
            printf("Test %d failed: %s (expected %d, got %d)\n", 
                   i + 1, test_cases[i].datetime, test_cases[i].ret_status, result);
        }
    }
    printf("%d/%d normalization tests passed.\n", passed, num_tests);

}

// int main()
// {
//     printf("Running ISO 8601 Validator Tests:\n");
//     test_validator();
    
//     printf("\nRunning ISO 8601 Normalizer Tests:\n");
//     test_normalizer();
    
//     return 0;
// }