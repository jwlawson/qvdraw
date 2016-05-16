/**
 * qv2gml.cc
 * Copyright 2014-2015 John Lawson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * Converts a matrix to gml format.
 */
#include <unistd.h>

#include <string>

#include "qv/dynkin.h"
#include "qv/quiver_matrix.h"

#include "graph_factory.h"

void usage() {
	std::cout << "qv2gml [-d dynkin | -m matrix]" << std::endl;
}

bool valid_dynkin(std::string matrix) {
	return cluster::dynkin::MAP.count(matrix) != 0;
}

cluster::QuiverMatrix get_matrix(bool dynkin, const std::string& matrix) {
	if(dynkin) {
		return cluster::dynkin::MAP.at(matrix);
	}
	return std::move(cluster::QuiverMatrix(matrix));
}

void output_gml(const cluster::QuiverMatrix& mat) {
	qvdraw::graph_factory::graph(mat).second->writeGML(std::cout);
}

int main(int argc, char* argv[]) {
	bool dynkin = false;
	bool matrix = false;
	std::string str;
	int c;

	while( (c=getopt(argc, argv, "m:d:")) != -1) {
		switch(c) {
			case 'm':
				matrix = true;
				str = optarg;
				break;
			case 'd':
				dynkin = true;
				str = optarg;
				break;
			case '?':
				usage();
				return 1;
			default:
				usage();
				return 2;
		}
	}
	if(dynkin && !valid_dynkin(str)) {
		std::cout << "Unrecognized matrix" << std::endl;
		return 1;
	}
	if(!matrix && !dynkin) {
		usage();
		return 1;
	}
	typedef cluster::QuiverMatrix Matrix;
	Matrix mat = get_matrix(dynkin, str);
	output_gml(mat);
	return 0;
}

