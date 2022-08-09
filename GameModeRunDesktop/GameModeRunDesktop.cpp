// GameModeRunSingleDesktopRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//





#include "windows.h"
#include "stdlib.h"

#include "iostream"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object


int main()
{

	// TODO: code your application's behavior here.
	while (true)
	{
		Sleep(1000);

		QUERY_USER_NOTIFICATION_STATE result;

		QUERY_USER_NOTIFICATION_STATE* ptr = &result;
		SHQueryUserNotificationState(ptr);
		std::cout << result;
		if (result == QUNS_RUNNING_D3D_FULL_SCREEN || result == QUNS_BUSY)
		{
			SendMessage(handle, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
		}
		else
		{
			std::cout << "Not works";
		}
	}
}
