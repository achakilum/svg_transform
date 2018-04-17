#include "glyph.hpp"

#include <sstream>
#include <fstream>
#include <iomanip>

Glyph::Glyph(std::string in_filename)
{
	import_svg(in_filename);
}


Glyph::Glyph(const Glyph& other)
{
	copy(other);
}

		
Glyph& Glyph::operator=(const Glyph& other)
{
	if (this == &other)
	{
		return (*this);
	}

	copy(other);

	return (*this);
}


Glyph::~Glyph()
{
	clear();
}


void Glyph::scale(double sx, double sy)
{
	svg.scale(sx, sy);
	bbox.scale(sx, sy);

	// Scaling of glyph's coordinate system (NOT just the glyph itself about its own coordinate system)

	width *= sx;
	height *= sy;

	h_bearing_x *= sx;
	h_bearing_y *= sy;
	h_advance *= sx;

	v_bearing_x *= sx;
	v_bearing_y *= sy;
	v_advance *= sy;

	origin_x *= sx;
	origin_y *= sy;
}


void Glyph::translate(double tx, double ty)
{
	svg.translate(tx, -ty);
	bbox.translate(tx, ty);

	// Translation of glyph's coordinate system (NOT just the glyph itself about its own coordinate system)

	origin_x += tx;
	origin_y += ty;
}


std::string Glyph::to_string()
{
	std::stringstream ss;
	ss.str("");

	ss << "Source name: " 	<< source_name		<< "\n";
	ss << "\n---------------------------\n"		<< "\n";

	ss << "BBOX_X_MIN " 	<< std::setprecision(25) 	<< bbox.get_x_min() 	<< "\n";
	ss << "BBOX_Y_MIN " 	<< std::setprecision(25)	<< bbox.get_y_min() 	<< "\n";
	ss << "BBOX_X_MAX " 	<< std::setprecision(25)	<< bbox.get_x_max() 	<< "\n";
	ss << "BBOX_Y_MAX " 	<< std::setprecision(25)	<< bbox.get_y_max() 	<< "\n";

	ss << "WIDTH " 		<< std::setprecision(25)	<< width	 	<< "\n";
	ss << "HEIGHT " 	<< std::setprecision(25)	<< height	 	<< "\n";

	ss << "H_BEARING_X " 	<< std::setprecision(25)	<< h_bearing_x		<< "\n";
	ss << "H_BEARING_Y " 	<< std::setprecision(25)	<< h_bearing_y		<< "\n";
	ss << "H_ADVANCE " 	<< std::setprecision(25)	<< h_advance		<< "\n";

	ss << "V_BEARING_X " 	<< std::setprecision(25)	<< v_bearing_x	 	<< "\n";
	ss << "V_BEARING_Y " 	<< std::setprecision(25)	<< v_bearing_y	 	<< "\n";
	ss << "V_ADVANCE " 	<< std::setprecision(25)	<< v_advance		<< "\n";

	ss << "ORIGIN_X " 	<< std::setprecision(25)	<< origin_x	 	<< "\n";
	ss << "ORIGIN_Y " 	<< std::setprecision(25)	<< origin_y;

	return ss.str();
}


void Glyph::import_svg(std::string in_filename)
{
	source_name = in_filename;
	svg.import_file(in_filename);

	double bbox_x_min, bbox_y_min;
	double bbox_x_max, bbox_y_max;

	std::string token;
	std::ifstream svg_file(in_filename);

	bool scan_metadata = false;

	if (svg_file.is_open())
	{
		//svg_file >> "<!--\n";
		std::getline(svg_file, token);
		//svg_file >> "BEGIN METADATA\n";
		std::getline(svg_file, token);
		//svg_file >> "\n";
		std::getline(svg_file, token);
		
		//svg_file >> "BBOX_X_MIN " 	>> x_min 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); bbox_x_min 	= atof(token.c_str());
		//svg_file >> "BBOX_Y_MIN " 	>> y_min 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); bbox_y_min 	= atof(token.c_str());
		//svg_file >> "BBOX_X_MAX " 	>> x_max 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); bbox_x_max 	= atof(token.c_str());
		//svg_file >> "BBOX_Y_MAX " 	>> y_max 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); bbox_y_max 	= atof(token.c_str());

		//svg_file >> "WIDTH " 		>> width 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); width	= atof(token.c_str());
		//svg_file >> "HEIGHT " 	>> height 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); height	= atof(token.c_str());

		//svg_file >> "H_BEARING_X " 	>> h_bearing_x 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); h_bearing_x	= atof(token.c_str());
		//svg_file >> "H_BEARING_Y " 	>> h_bearing_y 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); h_bearing_y	= atof(token.c_str());
		//svg_file >> "H_ADVANCE " 	>> h_advance 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); h_advance	= atof(token.c_str());

		//svg_file >> "V_BEARING_X " 	>> v_bearing_x 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); v_bearing_x	= atof(token.c_str());
		//svg_file >> "V_BEARING_Y " 	>> v_bearing_y 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); v_bearing_y	= atof(token.c_str());
		//svg_file >> "V_ADVANCE " 	>> v_advance 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); v_advance	= atof(token.c_str());

		//svg_file >> "ORIGIN_X " 	>> origin_x 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); origin_x	= atof(token.c_str());
		//svg_file >> "ORIGIN_Y " 	>> origin_y 	>> "\n";
		std::getline(svg_file, token, ' '); std::getline(svg_file, token); origin_y	= atof(token.c_str());
	}

	bbox = BBox(bbox_x_min, bbox_y_min, bbox_x_max, bbox_y_max);
	svg_file.close();
	
}


void Glyph::export_svg(std::string out_filename)
{
	std::string svg_markup 		= svg.to_string();
	std::string glyph_metadata 	= to_string();

	int metadata_str_idx = glyph_metadata.find("BBOX");
	glyph_metadata = glyph_metadata.substr(metadata_str_idx);

	std::ofstream svg_file(out_filename, std::ofstream::out | std::ofstream::trunc);

	svg_file << "<!--"		<< "\n";
	svg_file << "BEGIN METADATA"	<< "\n";
	svg_file			<< "\n";
	svg_file << glyph_metadata	<< "\n";
	svg_file			<< "\n";
	svg_file << "END METADATA"	<< "\n";
	svg_file << "-->"		<< "\n";
	svg_file			<< "\n";
	svg_file << svg_markup;

	svg_file.flush();
	svg_file.close();
	
}


void Glyph::clear()
{
	
}


double Glyph::get_width() const
{
	return width;
}


double Glyph::get_height() const
{
	return height;
}


double Glyph::get_hori_bearing_x() const
{
	return h_bearing_x;
}


double Glyph::get_hori_bearing_y() const
{
	return h_bearing_y;
}


double Glyph::get_hori_advance() const
{
	return h_advance;
}


double Glyph::get_vert_bearing_x() const
{
	return v_bearing_x;
}


double Glyph::get_vert_bearing_y() const
{
	return v_bearing_y;
}


double Glyph::get_vert_advance() const
{
	return v_advance;
}


double Glyph::get_origin_x() const
{
	return origin_x;
}


double Glyph::get_origin_y() const
{
	return origin_y;
}


SVG Glyph::get_svg() const
{
	return svg;
}


BBox Glyph::get_bounding_box() const
{
	return bbox;
}


std::string Glyph::get_source_name() const
{
	return source_name;
}


void Glyph::set_width(const double& new_width)
{
	width = new_width;
}


void Glyph::set_height(const double& new_height)
{
	height = new_height;
}


void Glyph::set_hori_bearing_x(const double& new_h_bearing_x)
{
	h_bearing_x = new_h_bearing_x;
}


void Glyph::set_hori_bearing_y(const double& new_h_bearing_y)
{
	h_bearing_y = new_h_bearing_y;
}


void Glyph::set_hori_advance(const double& new_h_advance)
{
	h_advance = new_h_advance;
}


void Glyph::set_vert_bearing_x(const double& new_v_bearing_x)
{
	v_bearing_x = new_v_bearing_x;
}


void Glyph::set_vert_bearing_y(const double& new_v_bearing_y)
{
	v_bearing_y = new_v_bearing_y;
}


void Glyph::set_vert_advance(const double& new_v_advance)
{
	v_advance = new_v_advance;
}


void Glyph::set_origin_x(const double& new_origin_x)
{
	origin_x = new_origin_x;
}


void Glyph::set_origin_y(const double& new_origin_y)
{
	origin_y = new_origin_y;
}


void Glyph::set_svg(const SVG& new_svg)
{
	svg = new_svg;
}


void Glyph::set_bounding_box(const BBox& new_bbox)
{
	bbox = new_bbox;
}


void Glyph::set_source_name(const std::string& new_source_name)
{
	source_name = new_source_name;
}


void Glyph::copy(const Glyph& other)
{
	width = other.width;
	height = other.height;

	h_bearing_x = other.h_bearing_x;
	h_bearing_y = other.h_bearing_y;
	h_advance = other.h_advance;

	v_bearing_x = other.v_bearing_x;
	v_bearing_y = other.v_bearing_y;
	v_advance = other.v_advance;

	origin_x = other.origin_x;
	origin_y = other.origin_y;

	svg = other.svg;
	bbox = other.bbox;
}

