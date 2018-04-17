#include "bbox.hpp"

#include <algorithm>

BBox::BBox()
{
	clear();
}


BBox::BBox(double x0, double y0, double x1, double y1)
{
	x_min = x0;
	y_min = y0;
	x_max = x1;
	y_max = y1;
}


BBox::BBox(const BBox& other)
{
	copy(other);
}


BBox& BBox::operator=(const BBox& other)
{
	if (this == &other)
	{
		return (*this);
	}

	copy(other);

	return (*this);
}


BBox::~BBox()
{
	clear();
}


void BBox::scale(double sx, double sy)
{
	x_min *= sx;
	y_min *= sy;
	x_max *= sx;
	y_max *= sy;
}


void BBox::translate(double tx, double ty)
{
	x_min += tx;
	y_min += ty;
	x_max += tx;
	y_max += ty;
}

BBox BBox::surrounding(const BBox& b1, const BBox& b2)
{	
	double x0 = std::min(b1.get_x_min(), b2.get_x_min());
	double y0 = std::min(b1.get_y_min(), b2.get_y_min());
	double x1 = std::max(b1.get_x_max(), b2.get_x_max());
	double y1 = std::max(b1.get_y_max(), b2.get_y_max());

	return BBox(x0, y0, x1, y1);
}


double BBox::get_x_min() const
{
	return x_min;
}


double BBox::get_y_min() const
{
	return y_min;
}


double BBox::get_x_max() const
{
	return x_max;
}


double BBox::get_y_max() const
{
	return y_max;
}


void BBox::clear()
{
	x_min = 0;
	y_min = 0;
	x_max = 1;
	x_max = 1;
}


void BBox::copy(const BBox& other)
{
	x_min = other.x_min;
	y_min = other.y_min;
	x_max = other.x_max;
	y_max = other.y_max;
}



