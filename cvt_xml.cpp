/*
 * cvt_xml.cpp
 *
 *  Created on: Apr 13, 2018
 *      Author: sean
 */


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "./lib_xml/pugiconfig.hpp"
#include "./lib_xml/pugixml.hpp"
#include "./lib_util/util_system.h"

using namespace std;
using namespace pugi;

typedef struct  _SampleTag
{
	int type;   // right: 1, left 0

	int x;
	int y;
	int w;
	int h;

}SampleTag;

void readXmlSampleTag(
	const char *xmlFile,
	vector<SampleTag>  &sampleList,
	const char* objName,
	char *imgName)
{
	sampleList.clear();

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(xmlFile);

	// cout << result.description() << endl;

	pugi::xml_node annotation = doc.child("annotation");
	xml_node filename = annotation.child("filename");
	std::string fnstr(filename.child_value());

	sprintf(imgName, "%s", fnstr.c_str());

	for(xml_node objects = doc.first_element_by_path("annotation/object");
			objects;
			objects = objects.next_sibling("object"))
	{
		SampleTag st;
		xml_node objectname = objects.child("name");
		xml_node objectbox = objects.child("bndbox");
		std::string obname(objectname.child_value());

		st.x = atoi(objectbox.child("xmin").child_value());
		st.y = atoi(objectbox.child("ymin").child_value());
		st.w = atoi(objectbox.child("xmax").child_value()) - st.x + 1;
		st.h = atoi(objectbox.child("ymax").child_value()) - st.y + 1;

		if(strncmp(obname.c_str(), objName, strlen(objName)) == 0)
		{
			sampleList.push_back(st);
		}

		
	}
}

int main(int argc, char** argv)
{
	const char* objName = "left";
	const char *pathName = "E:\\Projects\\CableInspector\\CableInspector\\data\\img_20180330";
	const char *cvDsFileName = "E:\\Projects\\CableInspector\\CableInspector\\data\\img_20180330\\left_data.txt";
	
	vector<string> fileNames;
	FILE *cvDsFile;

	getFiles(pathName, ".xml", fileNames);

	if(fileNames.size() <= 0)
	{
		return -1;
	}

	cvDsFile = fopen(cvDsFileName, "w");

	if(cvDsFile == NULL )
	{
		return -2;
	}

	

	for(size_t i = 0; i < fileNames.size(); i++)
	{
		char imgName[512];
		vector<SampleTag> sampleList;
	
		readXmlSampleTag(fileNames[i].c_str(), sampleList, objName, imgName);

		if(sampleList.size() <= 0)
		{
			continue;
		}

		fprintf(cvDsFile, "%s ", imgName);
		fprintf(cvDsFile, "%d ", sampleList.size());

		for( size_t m = 0; m < sampleList.size(); m++)
		{
			fprintf(cvDsFile, "%d %d %d %d ",
				sampleList[m].x, sampleList[m].y,
				sampleList[m].w, sampleList[m].h);
		}

		fprintf(cvDsFile, "\n");
		
	}

	fclose(cvDsFile);


	return 0;
}

