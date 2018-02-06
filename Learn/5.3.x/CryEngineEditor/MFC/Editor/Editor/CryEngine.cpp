
//
//#include <CryCommon/CryMath/Cry_Math.h>
////#include <Cry_Geo.h>
//#include <Cry3DEngine/I3DEngine.h>
//
//#include <CryCommon/CrySystem/ISystem.h>

#include "stdafx.h"
#include "CryEngine.h"








CryEngine::CryEngine()
{
}


CryEngine::~CryEngine()
{
}

//struct SSystemInitParams
//{
//	int i;
//};


BOOL CryEngine::Init()
{
	//m_hSystemHandle = LoadLibrary("CrySystem.dll");

	//if (!m_hSystemHandle)
	//{
	//	Cry_Error("CrySystem.DLL Loading Failed");
	//	return false;
	//}

//	PFNCREATESYSTEMINTERFACE pfnCreateSystemInterface = NULL;//		(PFNCREATESYSTEMINTERFACE)::GetProcAddress(m_hSystemHandle, "CreateSystemInterface");

	//SSystemInitParams sip;

	/*sip.bEditor = true;
	sip.bDedicatedServer = false;
	sip.bPreview = bPreviewMode;
	sip.bTestMode = bTestMode;
	sip.hInstance = AfxGetInstanceHandle();*/

	return TRUE;
}