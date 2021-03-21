#pragma once

#include <vector>

struct XY
{
	float m_X, m_Y;

	XY()
		:m_X(0.f),m_Y(0.f) {}

	XY(float x, float y)
		:m_X(x),m_Y(y) {}
};

struct XYZ : public XY
{
	float m_Z;

	XYZ()
		:XY(), m_Z(0.f) {}

	XYZ(float x, float y, float z)
		:XY(x,y),m_Z(z)	{}
};

struct XYZW : public XYZ
{
	float m_W;

	XYZW()
		:XYZ(), m_W(0.f) {}

	XYZW(float x, float y, float z, float w)
		:XYZ(x,y,z),m_W(w) {}
};

class Point
{
public:
	Point(float x);
	Point(float x, float y);
	Point(XY xy);
	Point(float x, float y, float z);
	Point(XYZ xyz);
	Point(float x, float y, float z, float w);
	Point(XYZW xyzw);
	~Point();

	float GetX();
	float GetY();
	XY GetXY();
	float GetZ();
	XYZ GetXYZ();
	float GetW();
	XYZW GetXYZW();

	void SetX(float val);
	void SetY(float val);
	void SetXY(float x, float y);
	void SetXY(XY xy);
	void SetZ(float val);
	void SetXYZ(float x, float y, float z);
	void SetXYZ(XYZ xyz);
	void SetW(float val);
	void SetXYZW(float x, float y, float z, float w);
	void SetXYZW(XYZW xyzw);
private:
	std::vector<float> m_Points;
};
