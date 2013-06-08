#include <iostream>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SVPToolBox.h"
#include "Charconvert.h"
#include "SubTransFormat.h"
#include "shooterclient.key"

#define SVP_REV_NUMBER	0
#define UNIQU_HASH_SIZE 512

//using namespace std;

CSVPToolBox toolbox;


void GetUrlByType(int32_t req_type , int iTryID,char* apiurl){

	//char apiurl[100]="https://www.shooter.cn/";
	strcpy(apiurl,"https://www.shooter.cn/");
	if(iTryID > 1 && iTryID <= 11)
	{
		if(iTryID >= 4)
		{
			int iSvrId = 4 + rand()%7;
			if(iTryID%2)
			{
				sprintf(apiurl,"https://splayer%d.shooter.cn/", iSvrId-1);
			}
			else
			{
				sprintf(apiurl,"http://splayer%d.shooter.cn/", iSvrId-1);
			}
		}else
		{
			sprintf(apiurl,"https://splayer%d.shooter.cn/", iTryID-1);
		}
	}else if(iTryID > 11) 
	{
		strcpy(apiurl,"http://svplayer.shooter.cn/");
	}

	switch(req_type){
/*
		case 'upda':
			apiurl.Append( "api/updater.php?ver=3.6" );
			break;
		case 'upsb':
			apiurl.Append( "api/subup.php" );
			break;*/

		case 'sapi':
			strcat(apiurl, "api/subapi.php" );//apiurl最好是数组，不然会出错
			break;
	}
	
	}

int SetCURLopt(CURL *curl)
{
	//struct curl_slist *headerlist=NULL;
	//static const char buf[] = "Expect:";

	/* Comment for Subddownload */
	//AppSettings& s = AfxGetAppSettings();

	char buff[1024];
	/* Comment for Subddownload */
	//if(s.szOEMTitle.IsEmpty()){
		sprintf( buff, "SPlayer Build %d", SVP_REV_NUMBER );
	//}else{
	//	CSVPToolBox svpToolBox;
	//	int iDescLen = 0;
	//	char *oem = svpToolBox.CStringToUTF8(s.szOEMTitle, &iDescLen);
	//	sprintf_s( buff, "SPlayer Build %d OEM%s", SVP_REV_NUMBER ,oem );
	//	free(oem);
	//}
	char buff_cookie[UNIQU_HASH_SIZE];
	memset(buff_cookie,0,UNIQU_HASH_SIZE);

	
	

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, buff);

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 40);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
	curl_easy_setopt(curl,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_0); //must use 1.0 for proxy

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	//curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv2);


    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

	//curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip"); not native supported. so dont use this option
	// MUST not have this line curl_easy_setopt(curl, CURLOPT_POST, ....);
	
	//headerlist = curl_slist_append(headerlist, buf); //WTF ??
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	return 0;
}




//int ExtractDataFromAiSubRecvBuffer_STL(std::wstring szFilePath,FILE* sAiSubRecvBuff)//不加-g 选项会断错误，跟szFilePath这个参数传递有关
int ExtractDataFromAiSubRecvBuffer_STL(FILE* sAiSubRecvBuff)
{
	char szSBuff[2] = {0,0};
	int ret = 0;
	fseek(sAiSubRecvBuff, 0, SEEK_SET); // move point yo begining of file

	if (fread(szSBuff, sizeof(char), 1, sAiSubRecvBuff) < 1)
		printf("Fail to retrive First Stat Code\n");

	int iStatCode = szSBuff[0];

    if (iStatCode <= 0)
    {
        if(iStatCode==-1)
            printf("No subtitle finded");
        return 0;
    }

    int j;
        for(j = 0; j < iStatCode; j++)
		{
			int exterr = toolbox.HandleSubPackage(sAiSubRecvBuff);
			if(exterr)
			{
                //ret = exterr;
				break;
			}
		}

    //fclose(sAiSubRecvBuff);//can't close

    return 0;
}




int main(int argc, char* argv[])
{
    if (argc!=2) {
        printf("Usage:GetSubtitle videopath\n");
        return 0;
    }

    printf("Please wait...\n");

    SubTransFormat SubTF;

    char* strfilepath=argv[1];

    //std::wstring filepath=L"/home/zhao/oz.the.great.and.powerful.2013.720p.bluray.x264-sparks.mkv";
    //char* strfilepath=WideChar2MultiByte(filepath.c_str());

    std::wstring filepath=MutliByte2WideChar(strfilepath);
    std::wstring filehash=SubTF.ComputerFileHash_STL(strfilepath);
	std::wstring shortname= SubTF.GetShortFileNameForSearch(filepath);
	        


    char* strfilehash=WideChar2MultiByte(filehash.c_str());
    char* strshortname=WideChar2MultiByte(shortname.c_str());

    std::wstring uniqueIDHash2=SubTF.genVHash(strfilepath,strfilehash);
    
    char* struniqueIDHash2=WideChar2MultiByte(uniqueIDHash2.c_str());
    
    printf("hash:%s\nshortname:%s\nuniqueIDHash:%s\n",strfilehash,strshortname,struniqueIDHash2);
	

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
    char* form;
	form=WideChar2MultiByte(filepath.c_str());
    curl_formadd(&formpost,	&lastptr, CURLFORM_COPYNAME, "pathinfo", CURLFORM_COPYCONTENTS, form,CURLFORM_END);
	free(form);

    form=WideChar2MultiByte(filehash.c_str());
	curl_formadd(&formpost,	&lastptr, CURLFORM_COPYNAME, "filehash", CURLFORM_COPYCONTENTS, form,CURLFORM_END);
    free(form);

	form=WideChar2MultiByte(uniqueIDHash2.c_str());
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "vhash", CURLFORM_COPYCONTENTS, form,CURLFORM_END);
	free(form);
	
    

    form=WideChar2MultiByte(shortname.c_str());
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "shortname", CURLFORM_COPYCONTENTS, form,CURLFORM_END);
    free(form);

    CURL* curl = curl_easy_init();
	SetCURLopt(curl);

    char url[100];

    char filename[L_tmpnam];
    tmpnam(filename);

	FILE* fp=fopen(filename,"wb+");

    //FILE* fp=tmpfile();
    if(fp==NULL) return 0;
    
    printf("Searching for subtitles\n\n");

	for (int i=1;i<=7;i++)
	{		

        GetUrlByType('sapi',i,url);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode res = curl_easy_perform(curl);
        if (res==0)
        {
			int respcode=0;
			curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &respcode);
			if (respcode==200)
			{
                ExtractDataFromAiSubRecvBuffer_STL(fp);
			}
			break;
        }
		else
		{
			long l_oserr = 0;
			curl_easy_getinfo(curl,CURLINFO_OS_ERRNO,&l_oserr);
			printf(curl_easy_strerror(res));
		}

	}
	curl_easy_cleanup(curl);


	std::wstring szSubFilePath;
	  for (int i = 0; i < toolbox.szaSubTmpFileList.size(); i++)
  {
    szSubFilePath = toolbox.getSubFileByTempid_STL(i,strfilepath);
    if (szSubFilePath == L"EXIST") printf("\nThe same subtitle already existed!\n");

    else if (!szSubFilePath.empty())
        {
      //szSubArray -> push_back(szSubFilePath);
      //CPath fnPath(szSubFilePath.c_str());
      //fnPath.StripPath();
      printf("\nSubtitle downloaded to %s\n\n",WideChar2MultiByte(szSubFilePath.c_str()));

      //iSubTotal++;
         }
        else
        {
         printf("\nFail to get sub file\n");
        }
  }
      return 0;
  }

