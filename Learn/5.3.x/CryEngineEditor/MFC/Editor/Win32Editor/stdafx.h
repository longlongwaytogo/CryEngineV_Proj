// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
//#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define _LAUNCHER

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_Launcher

// Insert your headers here
#include <CryCore/Platform/platform.h>
#include <algorithm>
#include <vector>

// enable memory pool usage
#define USE_NEWPOOL
#include <CryMemory/CryMemoryManager.h>

// Insert your headers here
