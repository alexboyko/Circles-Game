#pragma once

#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_RIGHT  2
#define MOUSE_BUTTON_MIDDLE 3

#define MOUSE_BUTTON_UP		0
#define MOUSE_BUTTON_DOWN	1

// the function signature for mouse button callbacks
typedef void (* GLmousebuttonfun)(int,int,int,int);

//struct GLWindow
//{
//	// dimensions
//	int width;
//	int height;
//
//	// state flags
//	bool fullScreen;
//	bool active;
//	bool running;
//};

bool GLWindowCreate(const char* title, int width, int height);
void GLWindowDestroy();

void GLWindowClientSize(int* width, int* height);

void GLWindowBuildFont(const char* name, GLuint* base);

bool GLWindowShouldClose();
void GLSetWindowShouldClose(int close);

void GLPollEvents();
void GLSwapBuffers();

void GLSetMouseButtonCallback(GLmousebuttonfun);