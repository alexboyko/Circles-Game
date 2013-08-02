#pragma once

class Circle
{
public:

	bool Hit(int x, int y) const;
	void Draw();
	void Update(double delta);

	double GetPositionX() const;
	double GetPositionY() const;
	int GetRadius() const;
	int GetScore() const;

	void SetPosition(double x, double y);
	void SetRadius(int r);
	void SetColor(GLint r, GLint g, GLint b);
	void SetSpeed(double speed);
	void SetScore(int score);

	Circle();
	Circle(int x, int y, int r);
	virtual ~Circle();

private:
	int m_iScore;
	double m_iPosX, m_iPosY;
	int m_iRadius;
	double m_dSpeed;
	GLbyte m_cColorR, m_cColorG, m_cColorB;
};

