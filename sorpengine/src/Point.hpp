#pragma once
#include <limits>
#include <cmath>

template<class T>
class Point
{
public:
	T x, y;

	Point(){}
	Point(T x, T y) : x(x), y(y){}

	Point operator -(const Point &v) const
	{
		return Point(x - v.x, y - v.y);
	}

	Point operator + (const Point &v) const
	{
		return Point(x + v.x, y + v.y);
	}

	const Point& operator -=(const Point &v)
	{
		x -= v.x;
		y -= v.y;
		return(*this);
	}

	const Point& operator +=(const Point &v)
	{
		x += v.x;
		y += v.y;
		return(*this);
	}

	bool operator ==(const Point& v) const
	{
		return (compareValue(x, v.x) && compareValue(y, v.y));
	}

	bool operator !=(const Point& v) const
	{
		return (!compareValue(x, v.x) || !compareValue(y, v.y));
	}

	bool IsZero() const
	{
		return (compareValue(x, 0) && compareValue(y, 0));
	}

	Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Point& Negate()
	{
		x = -x;
		y = -y;
		return(*this);
	}

	T DistanceTo(const Point& v) const
	{
		T fx = x - v.x;
		T fy = y - v.y;
		return (int)sqrt((fx*fx) + (fy*fy));
	}

private:
	template< class T >
	bool compareValue(T a, T b) const 
	{ 
		return false; 
	}

	template<>
	bool compareValue(float a, float b) const
	{
		return (abs(T) <= std::numeric_limits<T>::epsilon()) ? true : false;
	}

	template<>
	bool compareValue(int a, int b) const
	{
		return a == b;
	}
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;
