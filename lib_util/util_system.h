/*
 * util_system.h
 *
 *  Created on: Mar 22, 2018
 *      Author: sean
 */

#ifndef UTIL_UTIL_SYSTEM_H_
#define UTIL_UTIL_SYSTEM_H_

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void  __declspec(dllexport) getFiles(
		const char* path,
		const char* fileMode,
		std::vector<std::string> &files);

#endif /* UTIL_UTIL_SYSTEM_H_ */
