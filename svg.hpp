#ifndef __SVG_H__
#define __SVG_H__

#include <string>
#include <vector>

#define PI 3.141592653589793238462643383279502884

class SVG
{

	public:

		SVG();
		SVG(std::string in_filename);
		SVG(const SVG& other);
		SVG& operator=(const SVG& other);
		~SVG();

		void scale(double sx, double sy);
		void translate(double tx, double ty);
		void rotate(double degrees);

		std::string to_string();

		void import_file(std::string in_filename);
		void export_file(std::string out_filename);

	private:

		void clear();
		void copy(const SVG& other);

		std::vector<char> draw_commands;
		std::vector<std::vector<double>> draw_parameters; 


};

#endif
