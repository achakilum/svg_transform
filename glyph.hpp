#ifndef __GLYPH_H__
#define __GLYPH_H__

#include <string>
#include "svg.hpp"
#include "bbox.hpp"

class Glyph
{

	public:

		Glyph(std::string in_filename);
		Glyph(const Glyph& other);		
		Glyph& operator=(const Glyph& other);
		~Glyph();

		void scale(double sx, double sy);
		void translate(double tx, double ty);

		std::string to_string();

		void import_svg(std::string in_filename);
		void export_svg(std::string out_filename);

		double get_width() const;
		double get_height() const;

		double get_hori_bearing_x() const;
		double get_hori_bearing_y() const;
		double get_hori_advance() const;

		double get_vert_bearing_x() const;
		double get_vert_bearing_y() const;
		double get_vert_advance() const;

		double get_origin_x() const;
		double get_origin_y() const;

		void set_width(const double& new_width);
		void set_height(const double& new_height);

		void set_hori_bearing_x(const double& new_h_bearing_x);
		void set_hori_bearing_y(const double& new_h_bearing_y);
		void set_hori_advance(const double& new_h_advance);

		void set_vert_bearing_x(const double& new_v_bearing_x);
		void set_vert_bearing_y(const double& new_v_bearing_y);
		void set_vert_advance(const double& new_v_advance);

		void set_origin_x(const double& new_origin_x);
		void set_origin_y(const double& new_origin_y);

		void set_svg(const SVG& new_svg);
		void set_bounding_box(const BBox& new_bbox);
		void set_source_name(const std::string& new_source_name);

		SVG get_svg() const;
		BBox get_bounding_box() const;
		std::string get_source_name() const;

	private:

		void clear();
		void copy(const Glyph& other);

		double width, height;

		double h_bearing_x, h_bearing_y;
		double h_advance;

		double v_bearing_x, v_bearing_y;
		double v_advance;

		double origin_x, origin_y;

		SVG svg;
		BBox bbox;
		std::string source_name;

};

#endif
