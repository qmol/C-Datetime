#ifndef __datetime_util_h__
#define __datetime_util_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func_status.h"

FunctionStatus validate_iso8601(const char* datetime);
FunctionStatus normalize_iso8601(const char* datetime, char datetime_utc[25]);

void test_validator();
void test_normalizer();

#endif // __datetime_util_h__
