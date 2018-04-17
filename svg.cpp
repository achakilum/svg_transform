#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>

#include "svg.hpp"

SVG::SVG()
{
	clear();

	draw_commands.push_back('M');
	draw_commands.push_back('Z');

	draw_parameters.push_back(std::vector<double>(2, 0.0));
	draw_parameters.push_back(std::vector<double>(1, 0.0));
}

SVG::SVG(std::string in_filename)
{
	clear();
	import_file(in_filename);
}

SVG::SVG(const SVG& other)
{
	clear();
	copy(other);
}

SVG& SVG::operator=(const SVG& other)
{
	if (this == &other)
	{
		return (*this);
	}

	clear();
	copy(other);

	return (*this);
}

SVG::~SVG()
{
	clear();
}


void SVG::scale(double sx, double sy)
{
	for (int i = 0; i < draw_commands.size(); i++)
	{
		char cmd = draw_commands[i];

		if (cmd == 'M')
		{
			draw_parameters[i][0] *= sx;
			draw_parameters[i][1] *= sy;
		}
		else
		if (cmd == 'L')
		{
			draw_parameters[i][0] *= sx;
			draw_parameters[i][1] *= sy;
		}
		else
		if (cmd == 'Q')
		{
			draw_parameters[i][0] *= sx;
			draw_parameters[i][1] *= sy;
			draw_parameters[i][2] *= sx;
			draw_parameters[i][3] *= sy;
		}
	}
}

void SVG::translate(double tx, double ty)
{
	for (int i = 0; i < draw_commands.size(); i++)
	{
		char cmd = draw_commands[i];

		if (cmd == 'M')
		{
			draw_parameters[i][0] += tx;
			draw_parameters[i][1] += ty;
		}
		else
		if (cmd == 'L')
		{
			draw_parameters[i][0] += tx;
			draw_parameters[i][1] += ty;
		}
		else
		if (cmd == 'Q')
		{
			draw_parameters[i][0] += tx;
			draw_parameters[i][1] += ty;
			draw_parameters[i][2] += tx;
			draw_parameters[i][3] += ty;
		}
	}
}

void SVG::rotate(double degrees)
{
	double radians = (degrees * PI) / 180.0;

	for (int i = 0; i < draw_commands.size(); i++)
	{
		char cmd = draw_commands[i];

		if (cmd == 'M')
		{	
			double x = draw_parameters[i][0];
			double y = draw_parameters[i][1];

			draw_parameters[i][0] = (x * cos(radians)) - (y * sin(radians));
			draw_parameters[i][1] = (x * sin(radians)) + (y * cos(radians));
		}
		else
		if (cmd == 'L')
		{
			double x = draw_parameters[i][0];
			double y = draw_parameters[i][1];

			draw_parameters[i][0] = (x * cos(radians)) - (y * sin(radians));
			draw_parameters[i][1] = (x * sin(radians)) + (y * cos(radians));
		}
		else
		if (cmd == 'Q')
		{
			double x1 = draw_parameters[i][0];
			double y1 = draw_parameters[i][1];
			double x  = draw_parameters[i][2];
			double y  = draw_parameters[i][3];

			draw_parameters[i][0] = (x1 * cos(radians)) - (y1 * sin(radians));
			draw_parameters[i][1] = (x1 * sin(radians)) + (y1 * cos(radians));
			draw_parameters[i][2] = (x * cos(radians)) - (y * sin(radians));
			draw_parameters[i][3] = (x * sin(radians)) + (y * cos(radians));
		}
	}
}


std::string SVG::to_string()
{
	std::stringstream ss;
	ss.str("");

	ss << "<svg xmlns='http://www.w3.org/2000/svg' version='1.1'>" << "\n";
	ss << "<g>" << "\n";
	ss << "<path d=\'" << "\n";

	for (int i = 0; i < draw_commands.size(); i++)
	{
		ss << draw_commands[i] << " ";

		if (draw_commands[i] == 'Z')
		{
			ss << "\n\n";
			continue;
		}

		for (int j = 0; j < draw_parameters[i].size(); j++)
		{
			if (j != 0)
			{
				ss << " ";
			}

			ss << std::setprecision(25) << draw_parameters[i][j];
		}
		
		ss << "\n";
	}

	ss << "\'/>" << "\n";
	ss << "</g>" << "\n";
	ss << "</svg>";

	return ss.str();
}


void SVG::import_file(std::string in_filename)
{
	clear();

	std::string line;
	std::stringstream path_buffer;
	std::ifstream svg_file(in_filename);

	bool scan_path = false;

	if (svg_file.is_open())
	{
		while (std::getline(svg_file, line))

		{
			if (!scan_path && line.find("<path") != std::string::npos && line.find("d=\'") != std::string::npos)
			{
				scan_path = true;
				continue;
			}

			if (scan_path)
			{
				if (line.find("\'/>") != std::string::npos)
				{
					scan_path = false;
					break;
				}
				else
				{
					path_buffer << line << std::endl;
				}
			}
		}
	}

	path_buffer.seekg(0, path_buffer.beg);

	char cmd = 0;
	char temp = 0;
	double arg0 = 0.0;
	double arg1 = 0.0;
	double arg2 = 0.0;
	double arg3 = 0.0;

	while (!path_buffer.eof())
	{
		path_buffer >> temp;

		if (temp == 'M')
		{
			if (cmd == temp)
			{
				draw_commands.pop_back();
				draw_parameters.pop_back();
			}

			cmd = temp;

			path_buffer >> arg0;
			path_buffer >> temp;
			path_buffer >> arg1;

			std::vector<double> cmd_args(2, 0.0);
			cmd_args[0] = arg0;
			cmd_args[1] = arg1;
			
			draw_commands.push_back(cmd);
			draw_parameters.push_back(cmd_args);
		}
		else
		if (temp == 'L')
		{
			cmd = temp;

			path_buffer >> arg0;
			path_buffer >> temp;
			path_buffer >> arg1;

			std::vector<double> cmd_args(2, 0.0);
			cmd_args[0] = arg0;
			cmd_args[1] = arg1;
			
			draw_commands.push_back(cmd);
			draw_parameters.push_back(cmd_args);
		}
		else
		if (temp == 'Q')
		{
			cmd = temp;

			path_buffer >> arg0;
			path_buffer >> temp;
			path_buffer >> arg1;
			path_buffer >> arg2;
			path_buffer >> temp;
			path_buffer >> arg3;

			std::vector<double> cmd_args(4, 0.0);
			cmd_args[0] = arg0;
			cmd_args[1] = arg1;
			cmd_args[2] = arg2;
			cmd_args[3] = arg3;
			
			draw_commands.push_back(cmd);
			draw_parameters.push_back(cmd_args);
		}
		else
		if (temp == 'Z')
		{
			cmd = temp;
			std::vector<double> cmd_args(1, 0.0);

			draw_commands.push_back(cmd);
			draw_parameters.push_back(cmd_args);
		}
	}

	// remove trailing 'Z' command
	draw_commands.pop_back();
	draw_parameters.pop_back();

	svg_file.close();
}

void SVG::export_file(std::string out_filename)
{
	std::ofstream svg_file(out_filename, std::ofstream::out | std::ofstream::trunc);

	svg_file << to_string() << "\n";

	svg_file.flush();
	svg_file.close();
}

void SVG::clear()
{
	draw_commands.clear();
	draw_parameters.clear();
}

void SVG::copy(const SVG& other)
{
	for (int i = 0; i < other.draw_commands.size(); i++)
	{
		draw_commands.push_back(other.draw_commands[i]);
	}

	for (int i = 0; i < other.draw_parameters.size(); i++)
	{
		draw_parameters.push_back(other.draw_parameters[i]);
	}
}

