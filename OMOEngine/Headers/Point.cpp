#include "../Headers/Point.h"

Point::Point(float x)
{
	m_Points.push_back(x);
}
Point::Point(float x, float y)
{
	m_Points = { x, y };
}
Point::Point(XY xy)
{
	m_Points = { xy.m_X, xy.m_Y };
}
Point::Point(float x, float y, float z)
{
	m_Points = { x, y, z };
}
Point::Point(XYZ xyz)
{
	m_Points = { xyz.m_X, xyz.m_Y, xyz.m_Z };
}
Point::Point(float x, float y, float z, float w)
{
	m_Points = { x, y, z, w };
}
Point::Point(XYZW xyzw)
{
	m_Points = { xyzw.m_X, xyzw.m_Y, xyzw.m_Z, xyzw.m_W };
}
Point::~Point()
{
	m_Points.clear();
}

float Point::GetX()
{
	return *m_Points.begin();
}
float Point::GetY()
{
	return *(m_Points.begin() + 1);
}
XY Point::GetXY()
{
	return XY(*m_Points.begin(), *(m_Points.begin() + 1));
}
float Point::GetZ()
{
	return *(m_Points.begin() + 2);
}
XYZ Point::GetXYZ()
{
	return XYZ(*m_Points.begin(), *(m_Points.begin() + 1), *(m_Points.begin() + 2));
}
float Point::GetW()
{
	return *(m_Points.begin() + 3);
}
XYZW Point::GetXYZW()
{
	return XYZW(*m_Points.begin(), *(m_Points.begin() + 1), *(m_Points.begin() + 2), *(m_Points.begin() + 3));
}

void Point::SetX(float val)
{
	m_Points[0] = val;
}
void Point::SetY(float val)
{
	m_Points[1] = val;
}
void Point::SetXY(float x, float y)
{
	SetX(x);	SetY(y);
}
void Point::SetXY(XY xy)
{
	SetX(xy.m_X);	SetY(xy.m_Y);
}
void Point::SetZ(float val)
{
	m_Points[2] = val;
}
void Point::SetXYZ(float x, float y, float z)
{
	SetX(x);	SetY(y); SetZ(z);
}
void Point::SetXYZ(XYZ xyz)
{
	SetX(xyz.m_X);	SetY(xyz.m_Y); SetZ(xyz.m_Z);
}
void Point::SetW(float val)
{
	m_Points[3] = val;
}
void Point::SetXYZW(float x, float y, float z, float w)
{
	SetX(x);	SetY(y); SetZ(z); SetW(w);
}
void Point::SetXYZW(XYZW xyzw)
{
	SetX(xyzw.m_X);	SetY(xyzw.m_Y); SetZ(xyzw.m_Z); SetW(xyzw.m_W);
}