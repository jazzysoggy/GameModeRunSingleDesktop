// GameModeRunSingleDesktopRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//





#include "windows.h"
#include "stdlib.h"
#include <lowlevelmonitorconfigurationapi.h>
#include "iostream"
#include "vector"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int main()
{


	QUERY_USER_NOTIFICATION_STATE result;
	QUERY_USER_NOTIFICATION_STATE* ptr = &result;
	SHQueryUserNotificationState(ptr);
	QUERY_USER_NOTIFICATION_STATE lastResult = result;
	// TODO: code your application's behavior here.
	while (true)
	{
		Sleep(1000);

		QUERY_USER_NOTIFICATION_STATE* ptr = &result;
		SHQueryUserNotificationState(ptr);

		if (result != lastResult)
		{
			if (result == QUNS_RUNNING_D3D_FULL_SCREEN || result == QUNS_BUSY)
			{
				system("DisplaySwitch.exe /internal");
			}
			else
			{
				system("DisplaySwitch.exe /extend");
			}
		}

		lastResult = result;
	}
}
