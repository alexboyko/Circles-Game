#pragma once

class Circle
{
public:

	void Draw();
	void Update(double delta);

	double GetPositionX() const;
	double GetPositionY() const;
	int GetRadius() const;

	void SetPosition(double x, double y);
	void SetRadius(int r);
	void SetColor(GLint r, GLint g, GLint b);
	void SetSpeed(double speed);

	Circle();
	Circle(int x, int y, int r);
	virtual ~Circle();

private:
	double m_iPosX, m_iPosY;
	int m_iRadius;
	double m_dSpeed;
	GLbyte m_cColorR, m_cColorG, m_cColorB;
};

