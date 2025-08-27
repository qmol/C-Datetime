#ifndef __func_status_h__
#define __func_status_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
If a function returns an integer code, 
    0 always means success!
*/
typedef enum  {
    /* If function merely returns an execution status to indicate
        everything goes correctly. */
    RET_SUCCESS = 0,

    /* For validation type of function, return TRUE (1) or FALSE (0) */
    TRUE_STATUS  = 1,
    FALSE_STATUS = 0,

    DT_ERR_TOO_SHORT        = -1,
    DT_ERR_WRONG_SEPARATOR  = -2,
    DT_ERR_WRONG_YEAR       = -3,
    DT_ERR_WRONG_MONTH      = -4,
    DT_ERR_WRONG_DAY        = -5,
    DT_ERR_WRONG_HOUR       = -6,
    DT_ERR_WRONG_MINUTE     = -7,
    DT_ERR_WRONG_SECOND     = -8,
    DT_ERR_WRONG_TZ         = -9,
    DT_ERR_WRONG_TZ_HOUR    = -10,
    DT_ERR_WRONG_TZ_MINUTE  = -11,

    NULL_INPUT_POINTER    = -20,
    MEMORY_ALLOCATION_ERR = -21,

} FunctionStatus;

#endif // __func_status_h__
