#include "stdafx.h"
#include "Circle.h"
#define _USE_MATH_DEFINES
#include "Math.h"


Circle::Circle()
	: m_iPosX(0), m_iPosY(0), m_iRadius(0)
{
}

Circle::Circle(int x, int y, int r)
	: m_iPosX(x), m_iPosY(y), m_iRadius(r)
{
}

void Circle::SetColor(GLint r, GLint g, GLint b)
{
	m_cColorR = r;
	m_cColorG = g;
	m_cColorB = b;
}

void Circle::SetRadius(int r)
{
	m_iRadius = r;
}

double Circle::GetPositionX() const
{
	return m_iPosX;
}

double Circle::GetPositionY() const
{
	return m_iPosY;
}

int Circle::GetRadius() const
{
	return m_iRadius;
}

void Circle::SetPosition(double x, double y)
{
	m_iPosX = x;
	m_iPosY = y;
}

void Circle::SetSpeed(double speed)
{
	m_dSpeed = speed;
}

void Circle::Draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(m_iPosX, m_iPosY, 0.f);
	//glScalef(m_iRadius, m_iRadius, m_iRadius);

	glBegin(GL_TRIANGLE_FAN);

	glColor3b(m_cColorR, m_cColorG, m_cColorB);
	//glColor3f(1.f, 1.f, 0.f);

	glVertex2d(m_iPosX, m_iPosY);
	for (int i = 0; i < 99; i++)
	{
		// alpha = 2 * i / 98
		double a = i / 49.f;
		glVertex2d(m_iPosX + m_iRadius * cos(M_PI * a), m_iPosY + m_iRadius * sin(M_PI * a));
	}
//	glVertex2f(0.f, 600.f);
//	glVertex2f(0.f, 0.f);
//	glVertex2f(400.f, 600.f);
	glEnd();
}

void Circle::Update(double delta)
{
	m_iPosY -= delta * m_dSpeed;
}

Circle::~Circle()
{
}
