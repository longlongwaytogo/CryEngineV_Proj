// Win32Editor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Editor.h"

#include <CryCore/Platform/CryWindows.h>
// TODO:  在此处引用程序需要的其他头文件
#include <windows.h>
#include <WinUser.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define IDM_FIRSTCHILD   50000

LRESULT CALLBACK FrameWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK CloseEnumProc(HWND, LPARAM);
LRESULT CALLBACK HelloWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RectWndProc(HWND, UINT, WPARAM, LPARAM);

// structure for storing data unique to each Hello child window
typedef struct tagHELLODATA
{
	UINT     iColor;
	COLORREF clrText;
}
HELLODATA, *PHELLODATA;


// structure for storing data unique to each Rect child window

typedef struct tagRECTDATA
{
	short cxClient;
	short cyClient;
}
RECTDATA, *PRECTDATA;

// global variables

TCHAR     szAppName[] = TEXT("MDIDemo");
TCHAR     szFrameClass[] = TEXT("MdiFrame");
TCHAR     szHelloClass[] = TEXT("MdiHelloChild");
TCHAR     szRectClass[] = TEXT("MdiRectChild");
HINSTANCE hInst;
HMENU     hMenuInit, hMenuHello, hMenuRect;
HMENU     hMenuInitWindow, hMenuHelloWindow, hMenuRectWindow;

HWND     hwndFrame, hwndClient;
HWND     hwndDialog = NULL;




// dialog
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}






LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	static HWND        hwndClient;
	CLIENTCREATESTRUCT clientcreate;
	HWND               hwndChild;
	MDICREATESTRUCT    mdicreate;

	switch (message)
	{
	case WM_CREATE:           // Create the client window

		clientcreate.hWindowMenu = hMenuInitWindow;
		clientcreate.idFirstChild = IDM_FIRSTCHILD;

		hwndClient = CreateWindow(TEXT("MDICLIENT"), NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU)1, hInst,
			(PSTR)&clientcreate);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_FILE_NEWHELLO:       // Create a Hello child window

			mdicreate.szClass = szHelloClass;
			mdicreate.szTitle = TEXT("Hello");
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;

			hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDICREATE, 0,
				(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;

		case IDM_FILE_NEWRECT:        // Create a Rect child window

			mdicreate.szClass = szRectClass;
			mdicreate.szTitle = TEXT("Rectangles");
			mdicreate.hOwner = hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;

			hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDICREATE, 0,
				(LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			return 0;

		case IDM_FILE_CLOSE:          // Close the active window

			hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDIGETACTIVE, 0, 0);

			if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
				SendMessage(hwndClient, WM_MDIDESTROY,
				(WPARAM)hwndChild, 0);
			return 0;

		case ID_FILE_OPENDIALOG:
			if(hwndDialog)
				ShowWindow(hwndDialog,SW_SHOW);
			return 0;
		case IDM_APP_EXIT:            // Exit the program

			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

			// messages for arranging windows

		case IDM_WINDOW_TILE:
			SendMessage(hwndClient, WM_MDITILE, 0, 0);
			return 0;

		case IDM_WINDOW_CASCADE:
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			return 0;

		case IDM_WINDOW_ARRANGE:
			SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0);
			return 0;

		case IDM_WINDOW_CLOSEALL:     // Attempt to close all children

			EnumChildWindows(hwndClient, CloseEnumProc, 0);
			return 0;

		default:             // Pass to active child...

			hwndChild = (HWND)SendMessage(hwndClient,
				WM_MDIGETACTIVE, 0, 0);
			if (IsWindow(hwndChild))
				SendMessage(hwndChild, WM_COMMAND, wParam, lParam);

			break;        // ...and then to DefFrameProc
		}
		break;

	case WM_QUERYENDSESSION:
	case WM_CLOSE:                      // Attempt to close all children

		SendMessage(hwnd, WM_COMMAND, IDM_WINDOW_CLOSEALL, 0);

		if (NULL != GetWindow(hwndClient, GW_CHILD))
			return 0;

		break;   // i.e., call DefFrameProc 

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// Pass unprocessed messages to DefFrameProc (not DefWindowProc)

	return DefFrameProc(hwnd, hwndClient, message, wParam, lParam);
}

BOOL CALLBACK CloseEnumProc(HWND hwnd, LPARAM lParam)
{
	if (GetWindow(hwnd, GW_OWNER))         // Check for icon title
		return TRUE;

	SendMessage(GetParent(hwnd), WM_MDIRESTORE, (WPARAM)hwnd, 0);

	if (!SendMessage(hwnd, WM_QUERYENDSESSION, 0, 0))
		return TRUE;

	SendMessage(GetParent(hwnd), WM_MDIDESTROY, (WPARAM)hwnd, 0);
	return TRUE;
}

LRESULT CALLBACK HelloWndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	static COLORREF clrTextArray[] = { RGB(0,   0, 0), RGB(255, 0,   0),
		RGB(0, 255, 0), RGB(0, 0, 255),
		RGB(255, 255, 255) };
	static HWND     hwndClient, hwndFrame;
	HDC             hdc;
	HMENU           hMenu;
	PHELLODATA      pHelloData;
	PAINTSTRUCT     ps;
	RECT            rect;

	switch (message)
	{
	case WM_CREATE:
		// Allocate memory for window private data

		pHelloData = (PHELLODATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, sizeof(HELLODATA));
		pHelloData->iColor = IDM_COLOR_BLACK;
		pHelloData->clrText = RGB(0, 0, 0);
		SetWindowLong(hwnd, 0, (long)pHelloData);

		// Save some window handles

		hwndClient = GetParent(hwnd);
		hwndFrame = GetParent(hwndClient);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_COLOR_BLACK:
		case IDM_COLOR_RED:
		case IDM_COLOR_GREEN:
		case IDM_COLOR_BLUE:
		case IDM_COLOR_WHITE:
			// Change the text color

			pHelloData = (PHELLODATA)GetWindowLong(hwnd, 0);

			hMenu = GetMenu(hwndFrame);

			CheckMenuItem(hMenu, pHelloData->iColor, MF_UNCHECKED);
			pHelloData->iColor = wParam;
			CheckMenuItem(hMenu, pHelloData->iColor, MF_CHECKED);

			pHelloData->clrText = clrTextArray[wParam - IDM_COLOR_BLACK];

			InvalidateRect(hwnd, NULL, FALSE);
		}
		return 0;

	case WM_PAINT:
		// Paint the window

		hdc = BeginPaint(hwnd, &ps);

		pHelloData = (PHELLODATA)GetWindowLong(hwnd, 0);
		SetTextColor(hdc, pHelloData->clrText);

		GetClientRect(hwnd, &rect);

		DrawTextA(hdc, TEXT("Hello, World!"), -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_MDIACTIVATE:
		// Set the Hello menu if gaining focus

		if (lParam == (LPARAM)hwnd)
			SendMessage(hwndClient, WM_MDISETMENU,
			(WPARAM)hMenuHello, (LPARAM)hMenuHelloWindow);

		// Check or uncheck menu item

		pHelloData = (PHELLODATA)GetWindowLong(hwnd, 0);
		CheckMenuItem(hMenuHello, pHelloData->iColor,
			(lParam == (LPARAM)hwnd) ? MF_CHECKED : MF_UNCHECKED);

		// Set the Init menu if losing focus

		if (lParam != (LPARAM)hwnd)
			SendMessage(hwndClient, WM_MDISETMENU, (WPARAM)hMenuInit,
			(LPARAM)hMenuInitWindow);

		DrawMenuBar(hwndFrame);
		return 0;

	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		if (IDOK != MessageBox(hwnd, TEXT("OK to close window?"),
			TEXT("Hello"),
			MB_ICONQUESTION | MB_OKCANCEL))
			return 0;

		break;   // i.e., call DefMDIChildProc

	case WM_DESTROY:
		pHelloData = (PHELLODATA)GetWindowLong(hwnd, 0);
		HeapFree(GetProcessHeap(), 0, pHelloData);
		return 0;
	}
	// Pass unprocessed message to DefMDIChildProc

	return DefMDIChildProc(hwnd, message, wParam, lParam);
}
LRESULT CALLBACK RectWndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient, hwndFrame;
	HBRUSH      hBrush;
	HDC         hdc;
	PRECTDATA   pRectData;
	PAINTSTRUCT ps;
	int         xLeft, xRight, yTop, yBottom;
	short       nRed, nGreen, nBlue;

	switch (message)
	{
	case WM_CREATE:
		// Allocate memory for window private data

		pRectData = (PRECTDATA)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, sizeof(RECTDATA));

		SetWindowLong(hwnd, 0, (long)pRectData);

		// Start the timer going

		SetTimer(hwnd, 1, 250, NULL);

		// Save some window handles
		hwndClient = GetParent(hwnd);
		hwndFrame = GetParent(hwndClient);
		return 0;

	case WM_SIZE:             // If not minimized, save the window size

		if (wParam != SIZE_MINIMIZED)
		{
			pRectData = (PRECTDATA)GetWindowLong(hwnd, 0);

			pRectData->cxClient = LOWORD(lParam);
			pRectData->cyClient = HIWORD(lParam);
		}

		break;        // WM_SIZE must be processed by DefMDIChildProc

	case WM_TIMER:            // Display a random rectangle

		pRectData = (PRECTDATA)GetWindowLong(hwnd, 0);
		xLeft = rand() % pRectData->cxClient;
		xRight = rand() % pRectData->cxClient;
		yTop = rand() % pRectData->cyClient;
		yBottom = rand() % pRectData->cyClient;
		nRed = rand() & 255;
		nGreen = rand() & 255;
		nBlue = rand() & 255;

		hdc = GetDC(hwnd);
		hBrush = CreateSolidBrush(RGB(nRed, nGreen, nBlue));
		SelectObject(hdc, hBrush);

		Rectangle(hdc, min(xLeft, xRight), min(yTop, yBottom),
			max(xLeft, xRight), max(yTop, yBottom));

		ReleaseDC(hwnd, hdc);
		DeleteObject(hBrush);
		return 0;

	case WM_PAINT:            // Clear the window

		InvalidateRect(hwnd, NULL, TRUE);
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_MDIACTIVATE:      // Set the appropriate menu
		if (lParam == (LPARAM)hwnd)
			SendMessage(hwndClient, WM_MDISETMENU, (WPARAM)hMenuRect,
			(LPARAM)hMenuRectWindow);
		else
			SendMessage(hwndClient, WM_MDISETMENU, (WPARAM)hMenuInit,
			(LPARAM)hMenuInitWindow);

		DrawMenuBar(hwndFrame);
		return 0;

	case WM_DESTROY:
		pRectData = (PRECTDATA)GetWindowLong(hwnd, 0);
		HeapFree(GetProcessHeap(), 0, pRectData);
		KillTimer(hwnd, 1);
		return 0;
	}
	// Pass unprocessed message to DefMDIChildProc

	return DefMDIChildProc(hwnd, message, wParam, lParam);
}








#include <ShellAPI.h> // requires <windows.h>

// We need shell api for Current Root Extrection.
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#include <CryCore/Platform/CryLibrary.h>
#include <CryGame/IGameFramework.h>

#include <CrySystem/IConsole.h>
#include <CrySystem/File/ICryPak.h>

#include <CryCore/Platform/platform_impl.inl>
#include <CrySystem/Profilers/FrameProfiler/FrameProfiler_impl.h>
#include <CryString/StringUtils.h>

#include <CrySystem/ParseEngineConfig.h>

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
	// nVidia
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMD
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#ifdef _LIB
extern "C" IGameFramework* CreateGameFramework();
#endif //_LIB

#ifdef _LIB
// Include common type defines for static linking
// Manually instantiate templates as needed here.
#include <CryCore/Common_TypeInfo.h>
STRUCT_INFO_T_INSTANTIATE(Vec2_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Vec2_tpl, <int>)
STRUCT_INFO_T_INSTANTIATE(Vec4_tpl, <short>)
STRUCT_INFO_T_INSTANTIATE(Vec3_tpl, <int>)
STRUCT_INFO_T_INSTANTIATE(Ang3_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Quat_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Plane_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Matrix33_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Color_tpl, <float>)
STRUCT_INFO_T_INSTANTIATE(Color_tpl, <uint8>)
#endif

#define CRY_LIVE_CREATE_MUTEX_ENABLE 0

#if CRY_LIVE_CREATE_MUTEX_ENABLE
class CCryLiveCreateMutex
{
private:
	HANDLE m_hMutex;

public:
	CCryLiveCreateMutex(bool bCreateMutex)
		: m_hMutex(NULL)
	{
		if (bCreateMutex)
		{
			// create PC only mutex that signals that the game is running
			// this is used by LiveCreate to make sure only one instance of game is running
			const char* szMutexName = "Global\\CryLiveCreatePCMutex";
			m_hMutex = ::CreateMutexA(NULL, FALSE, szMutexName);
			if (NULL == m_hMutex)
			{
				// mutex may already exist, then just open it
				m_hMutex = ::OpenMutexA(MUTEX_ALL_ACCESS, FALSE, szMutexName);
			}
		}
	}

	~CCryLiveCreateMutex()
	{
		if (NULL != m_hMutex)
		{
			::CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}
};
#endif


int RunGame(const char* commandLine)
{
	SSystemInitParams startupParams;
	const char* frameworkDLLName = "CryAction.dll";

	CryFindRootFolderAndSetAsCurrentWorkingDirectory();

	//restart parameters
	static const size_t MAX_RESTART_LEVEL_NAME = 256;
	char fileName[MAX_RESTART_LEVEL_NAME];
	cry_strcpy(fileName, "");
	static const char logFileName[] = "Game.log";

#if CRY_LIVE_CREATE_MUTEX_ENABLE
	const bool bCreateMutex = (NULL == strstr(commandLine, "-nomutex"));
	CCryLiveCreateMutex liveCreateMutex(bCreateMutex);
#endif

	HMODULE frameworkDll = 0;

#if !defined(_LIB)
	// load the framework dll
	frameworkDll = CryLoadLibrary(frameworkDLLName);

	if (!frameworkDll)
	{
		string errorStr;
		errorStr.Format("Failed to load the Game DLL! %s", frameworkDLLName);

		MessageBox(0, errorStr.c_str(), "Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY);
		// failed to load the dll

		return 0;
	}

	// get address of startup function
	IGameFramework::TEntryFunction CreateGameFramework = (IGameFramework::TEntryFunction)CryGetProcAddress(frameworkDll, "CreateGameFramework");

	if (!CreateGameFramework)
	{
		// dll is not a compatible game dll
		CryFreeLibrary(frameworkDll);

		MessageBox(0, "Specified Game DLL is not valid! Please make sure you are running the correct executable", "Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY);

		return 0;
	}
#endif //!defined(_LIB)

	//SSystemInitParams startupParams;

	startupParams.sLogFileName = logFileName;
	cry_strcpy(startupParams.szSystemCmdLine, commandLine);
	//startupParams.pProtectedFunctions[0] = &TestProtectedFunction;
	startupParams.hWnd = hwndDialog;
	//startupParams.bEditor = true;

	// create the startup interface
	IGameFramework* pFramework = CreateGameFramework();
	if (!pFramework)
	{
		// failed to create the startup interface
		CryFreeLibrary(frameworkDll);

		const char* noPromptArg = strstr(commandLine, "-noprompt");
		if (noPromptArg == NULL)
			MessageBox(0, "Failed to create the GameStartup Interface!", "Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY);

		return 0;
	}

	bool oaRun = false;

	if (strstr(commandLine, "-norandom"))
		startupParams.bNoRandom = 1;

	// main game loop
	pFramework->StartEngine(startupParams);

	// The main engine loop has exited at this point, shut down
	pFramework->ShutdownEngine();

	// Unload the framework dll
	CryFreeLibrary(frameworkDll);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Support relaunching for windows media center edition.
//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS
bool ReLaunchMediaCenter()
{
	// Skip if not running on a Media Center
	if (GetSystemMetrics(SM_MEDIACENTER) == 0)
		return false;

	// Get the path to Media Center
	char szExpandedPath[MAX_PATH];
	if (!ExpandEnvironmentStrings("%SystemRoot%\\ehome\\ehshell.exe", szExpandedPath, MAX_PATH))
		return false;

	// Skip if ehshell.exe doesn't exist
	if (GetFileAttributes(szExpandedPath) == 0xFFFFFFFF)
		return false;

	// Launch ehshell.exe
	INT_PTR result = (INT_PTR)ShellExecute(NULL, TEXT("open"), szExpandedPath, NULL, NULL, SW_SHOWNORMAL);
	return (result > 32);
}
#endif // CRY_PLATFORM_WINDOWS


int APIENTRY CryWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// we need pass the full command line, including the filename
	// lpCmdLine does not contain the filename.
#if CAPTURE_REPLAY_LOG
#ifndef _LIB
	CryLoadLibrary("CrySystem.dll");
#endif
	CryGetIMemReplay()->StartOnCommandLine(lpCmdLine);
#endif

	//check for a restart
	const char* pos = strstr(lpCmdLine, "restart");
	if (pos != NULL)
	{
		Sleep(5000);               //wait for old instance to be deleted
		return RunGame(lpCmdLine); //pass the restart level if restarting
	}
	else
		pos = strstr(lpCmdLine, " -load ");// commandLine.find("load");

	if (pos != NULL)
		RunGame(lpCmdLine);

	int nRes = RunGame(GetCommandLineA());

	//////////////////////////////////////////////////////////////////////////
	// Support relaunching for windows media center edition.
	//////////////////////////////////////////////////////////////////////////
#if CRY_PLATFORM_WINDOWS
	if (strstr(lpCmdLine, "ReLaunchMediaCenter") != 0)
	{
		ReLaunchMediaCenter();
	}
#endif
	//////////////////////////////////////////////////////////////////////////

	return nRes;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	HACCEL   hAccel;
	
	MSG      msg;
	WNDCLASS wndclass;

	hInst = hInstance;

	// Register the frame window class

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = FrameWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szFrameClass;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}


	// Register the Hello child window class

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = HelloWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(HANDLE);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szHelloClass;

	RegisterClass(&wndclass);


	// Register the Rect child window class

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = RectWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(HANDLE);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szRectClass;

	RegisterClass(&wndclass);


	// Obtain handles to three possible menus & submenus

	hMenuInit = LoadMenu(hInstance, (LPCTSTR)MDIMENUINIT);
	hMenuHello = LoadMenu(hInstance, (LPCTSTR)MDIMENUHELLO);
	hMenuRect = LoadMenu(hInstance, (LPCTSTR)(MDIMENURECT));

	hMenuInitWindow = GetSubMenu(hMenuInit, INIT_MENU_POS);
	hMenuHelloWindow = GetSubMenu(hMenuHello, HELLO_MENU_POS);
	hMenuRectWindow = GetSubMenu(hMenuRect, RECT_MENU_POS);

	// Load accelerator table

	hAccel = LoadAccelerators(hInstance, szAppName);

	// Create the frame window
	hwndFrame = CreateWindow(szFrameClass, TEXT("MDI Demonstration"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, hMenuInit, hInstance, NULL);
	hwndClient = GetWindow(hwndFrame, GW_CHILD);

	ShowWindow(hwndFrame, iCmdShow);
	UpdateWindow(hwndFrame);

	// create Dialog 
	hwndDialog = CreateDialog(hInstance, (LPCSTR)IDD_DIALOG1,hwndFrame, About);
	ShowWindow(hwndDialog, SW_SHOWNORMAL);
	if (hwndDialog == NULL)
	{
		assert(0&&"Create Diailog err");// 
	}
#if 1
	CryWinMain(hInstance, hPrevInstance, szCmdLine, iCmdShow);
	//// Enter the modified message loop
#else
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateMDISysAccel(hwndClient, &msg) &&
			!TranslateAccelerator(hwndFrame, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#endif
	// Clean up by deleting unattached menus

	DestroyMenu(hMenuHello);
	DestroyMenu(hMenuRect);

	return 1;
}





//#define MAX_LOADSTRING 100
//
//// 全局变量: 
//HINSTANCE hInst;                                // 当前实例
//WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
//WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
//
//// 此代码模块中包含的函数的前向声明: 
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: 在此放置代码。
//
//    // 初始化全局字符串
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_WIN32EDITOR, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // 执行应用程序初始化: 
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32EDITOR));
//
//    MSG msg;
//
//    // 主消息循环: 
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  函数: MyRegisterClass()
////
////  目的: 注册窗口类。
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style          = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc    = WndProc;
//    wcex.cbClsExtra     = 0;
//    wcex.cbWndExtra     = 0;
//    wcex.hInstance      = hInstance;
//    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32EDITOR));
//    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32EDITOR);
//    wcex.lpszClassName  = szWindowClass;
//    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   函数: InitInstance(HINSTANCE, int)
////
////   目的: 保存实例句柄并创建主窗口
////
////   注释: 
////
////        在此函数中，我们在全局变量中保存实例句柄并
////        创建和显示主程序窗口。
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  目的:    处理主窗口的消息。
////
////  WM_COMMAND  - 处理应用程序菜单
////  WM_PAINT    - 绘制主窗口
////  WM_DESTROY  - 发送退出消息并返回
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // 分析菜单选择: 
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: 在此处添加使用 hdc 的任何绘图代码...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
