/*
 * util_system.cpp
 *
 *  Created on: Mar 22, 2018
 *      Author: sean
 */

#include "util_system.h"

#if __linux__
#include <unistd.h>
#include <dirent.h>

void getFiles(
		const char* path,
		const char* fileMode,
		std::vector<std::string> &files)
{
	DIR *dir = NULL;
	struct dirent * ptr = NULL;
	files.clear();
	dir = opendir(path);
	if( NULL == dir)
		return;

	while ((ptr=readdir(dir)) != NULL)
	{
		char temp[1024];

		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
		{
			continue;
		}
		else if(ptr->d_type == 8)    ///file
		{
			sprintf(temp, "%s/%s",path,ptr->d_name);
			std::string  str = std::string(temp, 0, strlen(temp) + 1);
//			printf("%s\n", str.c_str());
			int idx = str.find(fileMode, strlen(temp)- strlen(fileMode));

			if(idx > 0)
			{
				files.push_back(str);
			}
		}
		else if(ptr->d_type == 10)    ///link file
		{
			sprintf(temp, "%s/%s",path,ptr->d_name);
		}
		else if(ptr->d_type == 4)    ///dir
		{
			sprintf(temp, "%s/%s",path,ptr->d_name);
//			readFileList(base);
		}
	}

	closedir(dir);
}

#endif

#ifdef _WIN32 || _WIN64

#include <io.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>


std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char    *m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}

void getFiles(
		const char* path,
		const char* fileMode,
		std::vector<std::string> &files)
{
	// TODO
	char strTemp[2048];
	TCHAR tstrTemp[1024];
	std::string strPath = std::string(path);

	sprintf_s(strTemp, "%s\\*%s", path, fileMode);

	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	
#ifdef UNICODE  
	MultiByteToWideChar(CP_ACP, 0, strTemp, -1, tstrTemp, 1024);
#else  
	strcpy(tstrTemp, strTemp);
#endif  
	int i = 0;

	files.clear();
	hFind = FindFirstFile(tstrTemp, &wfd);

	do
	{
		std::string strName;
		std::string strFile;

		strName = WcharToChar(wfd.cFileName);
		strFile = strPath + "\\" + strName;
		
		files.push_back(strFile);

	} while (FindNextFile(hFind, &wfd));


}

#endif

