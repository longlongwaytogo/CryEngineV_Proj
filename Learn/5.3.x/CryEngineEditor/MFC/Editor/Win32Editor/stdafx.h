// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
//#include <windows.h>

// C ����ʱͷ�ļ�
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
