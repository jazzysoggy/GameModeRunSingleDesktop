// GameModeRunSingleDesktopRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//





#include "windows.h"
#include "stdlib.h"
#include "iostream"
#include "vector"
#include "winreg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LSTATUS ReadRegistry(HKEY hKeyParent, LPCWSTR spath, LPCWSTR subkey, DWORD* readData)
{
	HKEY hKey;
	LSTATUS nResult = ::RegOpenKeyEx(
		hKeyParent,
		spath,
		0,
		KEY_READ,
		&hKey
	);

	if (nResult == ERROR_SUCCESS)
	{

		DWORD data;
		DWORD len = sizeof(DWORD);//size of data
		nResult = ::RegQueryValueEx(
			hKey,
			subkey,
			NULL,
			NULL,
			(LPBYTE)(&data),
			&len
		);
		RegCloseKey(hKey);
		(*readData) = data;
	}

	return (nResult);
}

int main()
{


	QUERY_USER_NOTIFICATION_STATE result;
	QUERY_USER_NOTIFICATION_STATE* ptr = &result;
	SHQueryUserNotificationState(ptr);
	bool currentState = false;
	bool lastState = false;
	// TODO: code your application's behavior here.
	while (true)
	{
		Sleep(1000);

		const int BUFFER_SIZE = 1024;

		DWORD nBufferSize;

		LSTATUS nResult = ReadRegistry(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"RunningAppID", &nBufferSize);


		if (result == QUNS_RUNNING_D3D_FULL_SCREEN || nBufferSize != 0)
		{
			currentState = true;
		}
		else
		{
			currentState = false;
		}

		if (currentState != lastState)
		{
			if (currentState == true)
			{
				system("C:\\Windows\\System32\\DisplaySwitch.exe /internal");
			}
			else
			{
				system("C:\\Windows\\System32\\DisplaySwitch.exe /extend");
			}
		}

		lastState = currentState;
	}
}
