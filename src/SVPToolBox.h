#ifndef SVPTOOLBOX_H
#define SVPTOOLBOX_H

#include <vector>
#include <string>

class CSVPToolBox
{
public:
	CSVPToolBox(void);
	~CSVPToolBox(void);


	int unpackGZfile(char* fnin , char* fnout );

	//CString AnsiToCString(UINT codepag, char* szIn, int iLength);
	//char* CStringToUTF8(CString szIn, int* iDescLen, UINT codePage = CP_UTF8);
	//CString UTF8ToCString(char* szIn, int iLength);

	FILE* getTmpFileSteam();
	char* getTmpFileName();

	int ClearTmpFiles();
	int Char4ToInt(char* szBuf);
	std::wstring getSubFileByTempid_STL(int iTmpID, char *strPath);
	

	int HandleSubPackage(FILE* fp);
	//CStringArray szaSubDescs; //save raw desc data order by id
	//CStringArray szaSubTmpFileList; //save tmp file names order by id, format: ".srt;C:\tmp\blahblah;.idx;C:\tmp\blah2;.sub;C:\tmp\blah3"...
	std::vector<std::wstring> szaSubDescs;
	std::vector<std::wstring> szaSubTmpFileList;

	int ExtractSubFiles(FILE* fp);
	int ExtractEachSubFile(FILE* fp, int iSubPosId);
	char* ReadToPTCharByLength(FILE* fp, size_t length);

	std::wstring getVideoFileBasename(std::wstring szVidPath,
		std::vector<std::wstring>* szaPathInfo);



	int Explode(std::wstring szIn, std::wstring szTok,
		std::vector<std::wstring>* szaOut);
  
 
	bool ifFileExist_STL(std::wstring szPathname, bool evenSlowDriver = true);

	bool ifDirWritable_STL(std::wstring szDir);

	//CString GetTempDir();
	

	std::wstring DetectSubFileLanguage_STL(std::wstring fn);
  
	void filePutContent_STL(std::wstring szFilePath, std::wstring szData, bool bAppend = 0);

	int CopyFile(std::wstring sourcePath, std::wstring destPath);

private:
	//CStringArray szaTmpFileNames;
	char* szaTmpFileNames[5];
};




#endif
