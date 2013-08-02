#include "stdafx.h"
#include "CirclesApp.h"
#include "time.h"

int Random(int from, int to)
{
	double r = (double)rand() / (double)RAND_MAX;
	return floor(r * (to - from) + 0.5) + from;
}

// dirty hack, should be avoid in real applications
CirclesApp* app;
void MouseClickCallback(int button, int state, int x, int y)
{
	app->ProcessClick(x, y);	
}

CirclesApp::CirclesApp(int width, int height, const char* title) 
	: m_iWidth(width), m_iHeight(height), m_szTitle(title), m_iMaxSimultaneous(10)
{
	m_iScore = 0;
	m_pTimer = new Timer();
	// random generator
	srand(m_pTimer->GetAbsoluteTime() * 100);
}

bool CirclesApp::Init()
{
	// hack for callback
	app = this;
	// create opengl window
	bool res = GLWindowCreate(m_szTitle, m_iWidth, m_iHeight);
	// initialize font bitmap
	GLWindowBuildFont("Courier New", &m_uiFontBase);
	// actual window client area size
	GLWindowClientSize(&m_iWidth, &m_iHeight);
	// mouse button callback register
	GLSetMouseButtonCallback(MouseClickCallback);
	return res;
}

void CirclesApp::Run()
{
	double delta = 0;
	double prevTime = 0;

	// since window is resizeable don't need to change viewport and projection matrix
	glViewport(0, 0, m_iWidth, m_iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_iWidth, 0, m_iHeight, -1.f, 20.f);

	while (!GLWindowShouldClose())
	{
		Update(m_pTimer->GetTime() - prevTime);
		Render();

		GLPollEvents();
		GLSwapBuffers();

		prevTime = m_pTimer->GetTime();
		Sleep(2);
	}
}

void CirclesApp::Update(double delta)
{
	EraseInvisibleCircles();
	if (m_lCircles.size() < m_iMaxSimultaneous)
		m_lCircles.push_back(CreateCircle());

	for (std::list<Circle*>::iterator i = m_lCircles.begin(); i != m_lCircles.end(); ++i)
		(*i)->Update(delta);
}

void CirclesApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (std::list<Circle*>::iterator i = m_lCircles.begin(); i != m_lCircles.end(); ++i)
		(*i)->Draw();
	PrintScore(m_iScore);
}

void CirclesApp::ProcessClick(int x, int y)
{
	// invert Y axe
	y = m_iHeight - y;
	for (std::list<Circle*>::reverse_iterator i = m_lCircles.rbegin(); i != m_lCircles.rend();)
	{
		if ((*i)->Hit(x, y))
		{
			m_iScore += (*i)->GetScore();
			delete (*i);
			m_lCircles.erase(--(i.base()));
			return;
		}
		else ++i;
	}
}

void CirclesApp::PrintScore(int score)
{
	char text[256];
	sprintf_s(text, "Score: %d", score);
	glColor3f(1.f, 1.f, 1.f);
	glRasterPos2f(10.f, 10.f);
	glPushAttrib(GL_LIST_BIT);
	glListBase(m_uiFontBase - 32);						// set base character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// draw list text
	glPopAttrib();
}

CirclesApp::~CirclesApp()
{
	// Release resources
	GLWindowDestroy();
	for (std::list<Circle*>::iterator i = m_lCircles.begin(); i != m_lCircles.end(); ++i)
		delete (*i);
	delete m_pTimer;
}

void CirclesApp::EraseInvisibleCircles()
{
	for (std::list<Circle*>::iterator i = m_lCircles.begin(); i != m_lCircles.end(); )
		if ((*i)->GetPositionY() < -(*i)->GetRadius())
		{
			delete (*i);
			m_lCircles.erase(i++);
		}
		else ++i;
}

Circle* CirclesApp::CreateCircle()
{
	const int maxRadius = 80;
	const int minRadius = 10;
	const int maxSpeed = 500;
	const int minSpeed = 200;
	const int maxScore = 10;
	const int minScore = 1;

	Circle* c = new Circle();

	// Initialize random horizontal position and radius
	int r = Random(minRadius, maxRadius);
	int x = Random(r, m_iWidth - r);
	int y = m_iHeight - r;

	// Linear interpolatation of speed according to radius
	double speed = maxSpeed - ((double)(r - minRadius) / (maxRadius - minRadius)) * (maxSpeed - minSpeed);
	// Linear interpolatation of speed according to radius
	int score = maxScore - ((double)(r - minRadius) / (maxRadius - minRadius)) * (maxScore - minScore);

	c->SetPosition(x, y);
	c->SetRadius(r);
	c->SetColor(Random(-30, 120), Random(-30, 120), Random(-30, 120));
	c->SetSpeed(speed);
	c->SetScore(score);

	return c;
}