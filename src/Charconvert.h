#ifndef CHARCONVERT_H
#define CHARCONVERT_H
#include <stdio.h>

char* WideChar2MultiByte(const wchar_t*);
wchar_t* MutliByte2WideChar(const char* szIn);

#endif
