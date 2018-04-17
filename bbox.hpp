#ifndef __BBOX_H__
#define __BBOX_H__

class BBox
{
	public:

		BBox();
		BBox(double x0, double y0, double x1, double y1);
		BBox(const BBox& other);
		BBox& operator=(const BBox& other);
		~BBox();

		void scale(double sx, double sy);
		void translate(double tx, double ty);
		static BBox surrounding(const BBox& b1, const BBox& b2);
		
		double get_x_min() const;
		double get_y_min() const;
		double get_x_max() const;
		double get_y_max() const;

	private:

		void clear();
		void copy(const BBox& other);

		double x_min, y_min;
		double x_max, y_max;


};

#endif

