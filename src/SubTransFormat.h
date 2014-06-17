#ifndef SVPHASH_H
#define SVPHASH_H
#include <iostream>
#include <vector>

//#include "svplib.h"


class SubTransFormat
{
public:
	SubTransFormat(void);
	~SubTransFormat(void);
	//CString ComputerFileHash(CString szFilePath);
	std::wstring ComputerFileHash_STL(char *szFilePath);
	std::wstring GetShortFileNameForSearch2(std::wstring szFn);
	std::wstring GetShortFileNameForSearch(std::wstring szFnPath);
	std::wstring genVHash(const wchar_t* szTerm2, const wchar_t* szTerm3);
	std::wstring genVHash(const char* szTerm2, const char* szTerm3);
	//CString HexToString(BYTE* lpszMD5);
};

#endif
