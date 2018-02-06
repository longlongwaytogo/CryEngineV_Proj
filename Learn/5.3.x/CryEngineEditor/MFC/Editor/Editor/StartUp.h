#pragma once


// We need shell api for Current Root Extrection.

//




class StartUp
{
public:
	StartUp();
	~StartUp();

	static StartUp& GetInstance();

	int RunGame(const char* commandLine, HWND hwnd);
	int  CryWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, HWND hwnd = 0);
};

