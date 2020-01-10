#pragma once
#ifndef __Vector2D__
#define __Vector2D__

#include <math.h>
class Vector2D
{
public:
	Vector2D(float x, float y) : m_x(x), m_y(y) {}

	float GetX() { return m_x; }
	float GetY() { return m_y; }

	void SetX(float x) { m_x = x; }
	void SetY(float y) { m_y = y; }

	float Length() { return sqrt(pow(m_x, 2) + pow(m_y, 2)); }
	void Normalize()
	{
		float l = Length();
		if (l > 0) // we never should divide by 0
		{
			(*this) *= 1 / l;
		}
	}

private:

	float m_x;
	float m_y;

public:

	Vector2D operator+(const Vector2D& v2) const
	{
		return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
	}

	friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;

		return v1;
	}

	Vector2D operator*(float scalar)
	{
		return Vector2D(m_x * scalar, m_y * scalar);
	}

	Vector2D& operator*=(float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;

		return *this;
	}

	Vector2D operator-(const Vector2D& v2) const
	{
		return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
	}

	friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;

		return v1;
	}

	Vector2D operator/(float scalar)
	{
		return Vector2D(m_x / scalar, m_y / scalar);
	}

	Vector2D& operator/=(float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;

		return *this;
	}
};

static class WarspiteMath
{
public:
	// Linear interpolates between two points (v1 and v2) by the value of t [0-1]
	inline static Vector2D Lerp(Vector2D v1, Vector2D v2, double t)
	{
		return Vector2D(
			(1 - t) * v1.GetX() + t * v2.GetX(), // X
			(1 - t) * v1.GetY() + t * v2.GetY()  // Y
		);
	}
};

#endif /* defined(__Vector2D__) */