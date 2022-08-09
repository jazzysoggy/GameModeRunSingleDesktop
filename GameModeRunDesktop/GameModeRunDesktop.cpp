// GameModeRunSingleDesktopRun.cpp : This file contains the 'main' function. Program execution begins and ends there.
//




#include "windows.h"
#include "stdlib.h"
#include "ddraw.h"
#include "iostream"

#define DDERR_EXCLUSIVEMODEALREADYSET           MAKE_DDHRESULT( 581 )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object


int main()
{

	// TODO: code your application's behavior here.
	while (true)
	{
		_sleep(1000);

		LPDIRECTDRAW7 DD;
		HRESULT result = DD->TestCooperativeLevel();

		if (result == DDERR_EXCLUSIVEMODEALREADYSET)
		{
			std::cout << "Works";
		}
		else
		{
			std::cout << "Not works";
		}
	}
}
