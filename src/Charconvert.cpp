#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

char* WideChar2MultiByte(const wchar_t* szIn)
{
	size_t len=512;

	char* szOut=(char*)malloc(sizeof(char)*len);
	wcstombs( szOut, szIn, len);
	
	return szOut;
}


wchar_t* MutliByte2WideChar(const char* szIn)
{
    size_t len=strlen(szIn)+1;

	wchar_t* szOut=(wchar_t*)malloc(sizeof(wchar_t)*len);
	mbstowcs(szOut,szIn,len);
	return szOut;
}
