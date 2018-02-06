#pragma once


class CryEngine
{
public:
	CryEngine();
	~CryEngine();

public :
	BOOL Init();

private:
	HMODULE m_hSystemHandle;

};

