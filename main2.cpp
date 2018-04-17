#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "svg.hpp"
#include "glyph.hpp"
#include "bbox.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

std::vector<std::string> dir_tree_leaf_pathnames(std::string dir_path_name);
std::vector<std::string> dir_tree_subdir_pathnames(std::string dir_path_name);

int main(int argc, char** argv)
{	
	double final_width = 100;
	double final_height = 100;

	// Read SVG glyph objects
	
	std::vector<std::vector<Glyph>> glyphs;
	std::vector<std::string> in_subdir_pathnames = dir_tree_subdir_pathnames("in");
	std::vector<std::vector<std::string>> in_pathnames;
	std::vector<std::string> out_subdir_pathnames;
	std::vector<std::vector<std::string>> out_pathnames;

	std::vector<std::string> space_metrics_fontnames;
	std::vector<int> space_metrics_hori_advances;

	for (int i = 0; i < in_subdir_pathnames.size(); i++)
	{
		std::string in_subdir_pathname = in_subdir_pathnames[i];

		std::string out_subdir_pathname = "out" + in_subdir_pathname.substr(2);
		out_subdir_pathnames.push_back(out_subdir_pathname);

		in_pathnames.push_back(dir_tree_leaf_pathnames(in_subdir_pathname));

		std::vector<std::string> cur_out_pathnames;
		std::vector<Glyph> cur_glyphs;

		for (int j = 0; j < in_pathnames[i].size(); j++)
		{
			std::string in_pathname = in_pathnames[i][j];

			Glyph g(in_pathname);
			cur_glyphs.push_back(g);

			std::string out_pathname = "out" + in_pathname.substr(2);
			cur_out_pathnames.push_back(out_pathname);
		}

		out_pathnames.push_back(cur_out_pathnames);
		glyphs.push_back(cur_glyphs);
	}

	in_subdir_pathnames.clear();
	in_pathnames.clear();

	std::ifstream space_metrics_file("space_metrics.txt");
	char line[128];
	int line_type = 0;

	while(space_metrics_file.getline(line, 128))
	{
		if (line_type == 0)
		{
			std::string space_metrics_fontname(line);
			space_metrics_fontnames.push_back(space_metrics_fontname);
		}
		else
		if (line_type == 1)
		{
			int space_metrics_hori_advance = atoi(line);
			space_metrics_hori_advances.push_back(space_metrics_hori_advance);
		}

		line_type = (line_type + 1) % 3;
	}

	space_metrics_file.close();

	// Write SVG glyph objects

	fs::path out_dir("out");
	if (!fs::exists(out_dir))
	{
		fs::create_directory(out_dir);
	}

	std::ofstream metadata_file("out/metadata.txt", std::ofstream::out | std::ofstream::trunc);

	for (int i = 0; i < out_subdir_pathnames.size(); i++)
	{
		fs::path out_subdir(out_subdir_pathnames[i]);
		if (!fs::exists(out_subdir))
		{
			fs::create_directory(out_subdir);
		}
	}

	BBox test_bbox(final_width / 2, final_height / -2, final_width / 2, final_height / -2);

	// Export SVGs that preserve glyph proportions
	for (int i = 0; i < glyphs.size(); i++)
	{
		BBox cur_font_bbox = glyphs[i][0].get_bounding_box();		

		for (int j = 1; j < glyphs[i].size(); j++)
		{
			cur_font_bbox = BBox::surrounding(cur_font_bbox, glyphs[i][j].get_bounding_box());
		}

		double tx = -1.0 * cur_font_bbox.get_x_min();
		double ty = -1.0 * cur_font_bbox.get_y_min();
		double sx = final_width / (cur_font_bbox.get_x_max() - cur_font_bbox.get_x_min());
		double sy = final_height / (cur_font_bbox.get_y_max() - cur_font_bbox.get_y_min());
		double s_final = std::min(sx, sy);

		ptrdiff_t space_metrics_idx = std::find(space_metrics_fontnames.begin(),
							space_metrics_fontnames.end(),
							out_subdir_pathnames[i].substr(4)) - space_metrics_fontnames.begin();

		Glyph space("blank.svg");
		space.scale(0, 0);
		space.set_hori_advance(space_metrics_hori_advances[space_metrics_idx]);

		space.translate(tx, ty);
		space.scale(s_final, s_final);
		space.translate(0, -1.0 * final_height);

		space.export_svg(out_subdir_pathnames[i] + "/0x0020.svg");

		for (int j = 0; j < glyphs[i].size(); j++)
		{
			Glyph g = glyphs[i][j];
			g.translate(tx, ty);
			g.scale(s_final, s_final);
			g.translate(0, -1.0 * final_height);

			test_bbox = BBox::surrounding(test_bbox, g.get_bounding_box());
			g.export_svg(out_pathnames[i][j]);
		}

		metadata_file << out_subdir_pathnames[i].substr(4) << "\n";
		metadata_file << "TRANSLATE " << std::setprecision(25) << tx << " " << ty << "\n";
		metadata_file << "SCALE " << std::setprecision(25) << s_final << " " << s_final << "\n";
		metadata_file << "TRANSLATE " << std::setprecision(25) << 0 << " " << (-1.0 * final_height) << "\n\n";
	}

	metadata_file.flush();
	metadata_file.close();

	return 0;
}

std::vector<std::string> dir_tree_leaf_pathnames(std::string dir_path_name)
{
	std::vector<std::string> leaf_pathnames;
	fs::path dir_path(dir_path_name);

	if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
	{
		return leaf_pathnames;
	}

	fs::directory_iterator end;
	for (fs::directory_iterator cur(dir_path); cur != end; cur++)
	{
		try
		{
			if (fs::is_directory(cur->path()))
			{
				std::vector<std::string> subtree_leaf_pathnames = dir_tree_leaf_pathnames(cur->path().string());
				leaf_pathnames.insert(std::end(leaf_pathnames), std::begin(subtree_leaf_pathnames), std::end(subtree_leaf_pathnames));
			}
			else
			{
				leaf_pathnames.push_back(cur->path().string());
			}
		}
		catch (const std::exception & ex)
		{
			std::cout << cur->path().string() << " : " << ex.what() << std::endl;
		}
	}

	return leaf_pathnames;
}

std::vector<std::string> dir_tree_subdir_pathnames(std::string dir_path_name)
{
	std::vector<std::string> subdir_pathnames;
	fs::path dir_path(dir_path_name);

	if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
	{
		return subdir_pathnames;
	}

	fs::directory_iterator end;
	for (fs::directory_iterator cur(dir_path); cur != end; cur++)
	{
		try
		{
			if (fs::is_directory(cur->path()))
			{
				subdir_pathnames.push_back(cur->path().string());
			}
		}
		catch (const std::exception & ex)
		{
			std::cout << cur->path().string() << " : " << ex.what() << std::endl;
		}
	}

	return subdir_pathnames;
}


