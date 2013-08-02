#include "stdafx.h"
#include "CirclesApp.h"
#include "time.h"

int Random(int from, int to)
{
	double r = (double)rand() / (double)RAND_MAX;
	return floor(r * (to - from) + 0.5) + from;
}

CirclesApp::CirclesApp(int width, int height, const char* title) 
	: m_iWidth(width), m_iHeight(height), m_szTitle(title), m_iMaxSimultaneous(10)
{
	m_pTimer = new Timer();
	// random generator
	srand(m_pTimer->GetAbsoluteTime() * 100);
}

bool CirclesApp::Init()
{
	bool res = GLWindowCreate(m_szTitle, m_iWidth, m_iHeight);
	// actual window client area size
	GLWindowClientSize(&m_iWidth, &m_iHeight);
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

	Circle* c = new Circle();

	// Initialize random horizontal position and radius
	int r = Random(minRadius, maxRadius);
	int x = Random(r, m_iWidth - r);
	int y = m_iHeight - r;

	// Linear interpolatation of speed according to radius
	double speed = maxSpeed - ((double)(r - minRadius) / (maxRadius - minRadius)) * (maxSpeed - minSpeed);
	
	c->SetPosition(x, y);
	c->SetRadius(r);
	c->SetColor(Random(-30, 120), Random(-30, 120), Random(-30, 120));
	c->SetSpeed(speed);

	return c;
}