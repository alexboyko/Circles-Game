#pragma once

#include "Timer.h"
#include "Circle.h"
#include "Font.h"
#include "GLWindow.h"
#include "math.h"
#include <list>

class CirclesApp
{
public:
	bool Init();
	void Run();

	CirclesApp(int width, int height, const char* title);
	~CirclesApp();

protected:
	void Update(double delta);
	void Render();
	void PrintScore(int score);
	void EraseInvisibleCircles();
	Circle* CreateCircle();

private:
	std::list<Circle*> m_lCircles;
	Font* m_pFont;
	const int m_iMaxSimultaneous;
	Timer* m_pTimer;
	int m_iScore;
	int m_iWidth, m_iHeight;
	int m_iClientWidth, m_iClientHeight;
	const char* m_szTitle;
	GLuint m_uiFontBase;
};

