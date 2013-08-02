// Circles.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Circles.h"
#include "GLWindow.h"
#include "CirclesApp.h"

// platform specific entry point
#ifdef _WIN32

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	CirclesApp* app = new CirclesApp(400, 600, "Circles Game");
	// window creation and context binding
	if (!app->Init())
	{
		delete app;
		return 1;
	}

	// main cycle
	app->Run();

	// destroy application
	delete app;

	return 0;
}

#endif