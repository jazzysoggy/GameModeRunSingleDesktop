// GameModeRunSingleDesktopRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//





#include "windows.h"
#include "stdlib.h"
#include "iostream"
#include "vector"
#include "winreg.h"
#include <string>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <fstream>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


std::vector<std::string> blacklist{};

std::vector<std::string> whitelist{};

namespace fs = std::filesystem;

bool isRunning(LPCSTR pName)
{
	HWND hwnd;
	hwnd = FindWindowA(NULL, pName);
	if (hwnd != 0) {
		return true;
	}
	else {
		return false;
	}
}

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
class Coordinate
{
public:
	int x;
	int y;
	Coordinate(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

std::vector<DISPLAYCONFIG_PATH_INFO> ClonePathInfoArray;
std::vector<DISPLAYCONFIG_MODE_INFO> CloneModeInfoArray;

void DetachDisplay()
{
	UINT32 NumPathArrayElements = 0;
	UINT32 NumModeInfoArrayElements = 0;
	LONG error = GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &NumModeInfoArrayElements);
	std::vector<DISPLAYCONFIG_PATH_INFO> PathInfoArray(NumPathArrayElements);
	std::vector<DISPLAYCONFIG_MODE_INFO> ModeInfoArray(NumModeInfoArrayElements);
	error = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &PathInfoArray[0], &NumModeInfoArrayElements, &ModeInfoArray[0], NULL);
	ClonePathInfoArray = PathInfoArray;
	CloneModeInfoArray = ModeInfoArray;
	for (unsigned int i = 0; i < PathInfoArray.size(); ++i) {
		if (PathInfoArray[i].sourceInfo.modeInfoIdx < ModeInfoArray.size()) {
			int modeIndex = PathInfoArray[i].sourceInfo.modeInfoIdx;
			_POINTL pos = ModeInfoArray[modeIndex].sourceMode.position;
			if (pos.x != 0 || pos.y != 0) {

				PathInfoArray[i].flags = 0;
				break;
			}


		}
	}
	error = SetDisplayConfig(NumPathArrayElements, &PathInfoArray[0], NumModeInfoArrayElements, &ModeInfoArray[0], (SDC_APPLY | SDC_ALLOW_CHANGES | SDC_USE_SUPPLIED_DISPLAY_CONFIG));
}

bool DetectDisplay()
{
	UINT32 NumPathArrayElements = 0;
	UINT32 NumModeInfoArrayElements = 0;
	LONG error = GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &NumModeInfoArrayElements);
	std::vector<DISPLAYCONFIG_PATH_INFO> PathInfoArray(NumPathArrayElements);
	std::vector<DISPLAYCONFIG_MODE_INFO> ModeInfoArray(NumModeInfoArrayElements);
	error = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &PathInfoArray[0], &NumModeInfoArrayElements, &ModeInfoArray[0], NULL);
	if (PathInfoArray.size() > 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AttachDisplay()
{
	SetDisplayConfig(ClonePathInfoArray.size(), &ClonePathInfoArray[0], CloneModeInfoArray.size(), &CloneModeInfoArray[0], (SDC_APPLY | SDC_ALLOW_CHANGES | SDC_USE_SUPPLIED_DISPLAY_CONFIG));
}

int OutputMonitors()
{
	UINT32 NumPathArrayElements = 0;
	UINT32 NumModeInfoArrayElements = 0;
	LONG error = GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &NumModeInfoArrayElements);
	std::vector<DISPLAYCONFIG_PATH_INFO> PathInfoArray(NumPathArrayElements);
	std::vector<DISPLAYCONFIG_MODE_INFO> ModeInfoArray(NumModeInfoArrayElements);
	error = QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &NumPathArrayElements, &PathInfoArray[0], &NumModeInfoArrayElements, &ModeInfoArray[0], NULL);
	return PathInfoArray.size();
}

std::string selectedMenu;
int numberOfOptions = 0;
int selectedOption = 1;

std::vector<const char*> mainMenu{ "[*] Enable/Disable","[ ] Choose Monitor","[ ] Add Application To Whitelist","[ ] Remove Application From Whitelist","[ ] Quit" };
std::vector<const char*> monitorMenu{ "Select Monitor" };
std::vector<const char*> applicationMenu{ "Input Application Name" };
std::vector<const char*> removeMenu{ "Select Application To Remove" };
void Display(int index)
{
	switch (index) {
	case 0:
		for (int i = 1; i < mainMenu.size(); i++)
		{
			std::cout << mainMenu[i] << std::endl;
		}
		numberOfOptions = mainMenu.size();
		selectedMenu = "main";
		break;
	case 2:
		for (int i = 1; i < monitorMenu.size(); i++)
		{
			std::cout << monitorMenu[i] << std::endl;
		}

		numberOfOptions = OutputMonitors();
		selectedMenu = "monitor";
		break;
	case 3:

		break;
	case 4:
		break;
	}
}



int main()
{


	QUERY_USER_NOTIFICATION_STATE result;
	QUERY_USER_NOTIFICATION_STATE* ptr = &result;
	SHQueryUserNotificationState(ptr);
	bool currentState = false;
	bool lastState = false;

	int currentSelection = 0;
	int lastSelection = 1;

	bool whiteExists = fs::exists("whitelist.txt");

	system("cd");

	if (whiteExists)
	{
		std::ifstream infile("whitelist.txt");
		if (infile.is_open())
		{

			std::istream_iterator<std::string> start(infile), end;

			std::vector<std::string> temp(start, end);

			whitelist = temp;
		}

		infile.close();
	}
	else
	{
		std::ofstream  outfile("whitelist.txt");
		outfile << "Roblox" << std::endl;

		whitelist.push_back("Roblox");
		
		outfile.close();
	}

	// TODO: code your application's behavior here.
	while (true)
	{
		Sleep(1000);

		const int BUFFER_SIZE = 1024;

		DWORD nBufferSize;

		LSTATUS nResult = ReadRegistry(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"RunningAppID", &nBufferSize);

		if (nBufferSize != 0 || result == QUNS_RUNNING_D3D_FULL_SCREEN)
		{
			currentState = true;
		}
		else
		{
			currentState = false;
		}

		if (currentState == false)
		{
			for (int i = 0; i < whitelist.size(); i++)
			{
				if (isRunning(whitelist[i].c_str()))
				{
					currentState = true;
					break;
				}
			}
		}

		if (currentState == lastState)
		{
			if (currentState && DetectDisplay())
			{
				DetachDisplay();
			}
			else
			{
				AttachDisplay();
			}
		}
		lastState = DetectDisplay();

		if (currentSelection != lastSelection)
		{

		}


		lastSelection = currentSelection;
	}
}
