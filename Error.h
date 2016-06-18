#ifndef ERROR_H
#define ERROR_H

#include "WString.h"

void setupErrors();

void warning(const __FlashStringHelper* str);
void error(const __FlashStringHelper* str);
void fatal(const __FlashStringHelper* str);

#endif