#include "stdafx.h"
#include "GLWindow.h"

HDC              g_hDC;											// device context of window
HWND             g_hWnd;										// window handle
HGLRC            g_hGLRC;										// OpenGL rendering context
HINSTANCE        g_hInst;										// current instance
TCHAR            g_szWindowClass[] = _T("CIRCLES_GAME_CLASS");	// the main window class name
GLmousebuttonfun g_fMouseButtonCallback;						// the callback for mouse button press event
bool             g_bWindowShouldClose;							// flag indicating that window should be closed
//GLWindow  g_window;

// Forward declarations
ATOM				RegisterWindowClass(HINSTANCE);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				InitInstance(const char*, int, int);
WCHAR*				_createWideStringFromUTF8(const char* source);

//
//  FUNCTION: GLWindowCreate()
//
//  PURPOSE: Creates the window instance initialized with given params.
//
bool GLWindowCreate(const char *title, int width, int height)
{
	assert(title);
	assert(width > 0);
	assert(height > 0);

	g_hInst = static_cast<HINSTANCE>(GetModuleHandle(NULL));

	RegisterWindowClass(g_hInst);

	return static_cast<bool>(InitInstance(title, width, height));
}

void GLWindowDestroy()
{
	
}

void GLWindowClientSize(int* width, int* height)
{
	RECT rcClient;
	GetClientRect(g_hWnd, &rcClient);
	*width = rcClient.right - rcClient.left;
	*height = rcClient.bottom - rcClient.top;
}

void GLPollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
			g_bWindowShouldClose = true;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void GLSwapBuffers()
{
	SwapBuffers(g_hDC);
}

bool GLWindowShouldClose()
{
	return g_bWindowShouldClose;
}

void GLSetWindowShouldClose(int value)
{
	g_bWindowShouldClose = value;
}

void GLSetMouseButtonCallback(GLmousebuttonfun callback)
{
	g_fMouseButtonCallback = callback;
}

void GLWindowBuildFont(const char* name, GLuint* base)
{
	// font handles
	HFONT	hFont;
	HFONT	hOldFont;

	// create diplay list for font
	*base = glGenLists(96);

	WCHAR* wName = _createWideStringFromUTF8(name);

	hFont = CreateFont(
		-24,							// height of font
		0,								// width of font
		0,								// angle of escapement
		0,								// orientation angle
		FW_BOLD,						// font weight
		FALSE,							// italic
		FALSE,							// underline
		FALSE,							// strikeout
		ANSI_CHARSET,					// character set identifier
		OUT_TT_PRECIS,					// output precision
		CLIP_DEFAULT_PRECIS,			// clipping precision
		ANTIALIASED_QUALITY,			// output quality
		FF_DONTCARE | DEFAULT_PITCH,	// family and pitch
		wName);							// font name

	hOldFont = (HFONT)SelectObject(g_hDC, hFont);
	wglUseFontBitmaps(g_hDC, 32, 96, *base);
	// restore previous font
	SelectObject(g_hDC, hOldFont);
	DeleteObject(hFont);
}

//
//  FUNCTION: RegisterWindowClass()
//
//  PURPOSE: Registers the window class.
//
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLES));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName	= g_szWindowClass;

	return RegisterClassEx(&wcex);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (g_fMouseButtonCallback)
		{
			if (message == WM_LBUTTONDOWN)
				g_fMouseButtonCallback(MOUSE_BUTTON_LEFT, MOUSE_BUTTON_DOWN);
			else if (message == WM_RBUTTONDOWN)
				g_fMouseButtonCallback(MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_DOWN);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Creates main window
//
//
BOOL InitInstance(const char* title, int width, int height)
{
	// Window styles
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	WCHAR* wTitle = _createWideStringFromUTF8(title);

	g_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,			// Force onto the taskbar when visible
		g_szWindowClass,			// Registered window class
		wTitle,						// Window title
		style,						// Window style
		x, y,						// Position
		width, height,				// Size
		NULL, NULL, g_hInst, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	g_hDC = GetDC(g_hWnd);

	if (!g_hDC)
	{
		return FALSE;
	}

	// Choose appropriate pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = 32;
	pfd.cDepthBits   = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType   = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(g_hDC, &pfd);
	if (!iPixelFormat || !SetPixelFormat(g_hDC, iPixelFormat, &pfd))
	{
		return FALSE;
	}

	// OpenGL context creation
	g_hGLRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hGLRC);

	ShowWindow(g_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hWnd);

	return TRUE;
}


// Returns a wide string version of the specified UTF-8 string
//
WCHAR* _createWideStringFromUTF8(const char* source)
{
	WCHAR* target;
	int length;

	length = MultiByteToWideChar(CP_UTF8, 0, source, -1, NULL, 0);
	if (!length)
		return NULL;

	target = new WCHAR[length + 1];

	if (!MultiByteToWideChar(CP_UTF8, 0, source, -1, target, length + 1))
	{
		delete[] target;
		return NULL;
	}

	return target;
}